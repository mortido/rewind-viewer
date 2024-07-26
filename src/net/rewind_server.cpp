#include "net/rewind_server.h"

#include <rapidjson/error/en.h>

#include "common/logger.h"

namespace {

void fix_AABB(glm::vec2& min_corner, glm::vec2& max_corner) {
  if (min_corner.x > max_corner.x) {
    std::swap(min_corner.x, max_corner.x);
  }
  if (min_corner.y > max_corner.y) {
    std::swap(min_corner.y, max_corner.y);
  }
}

}  // namespace

namespace rewind_viewer::net {

RewindServer::RewindServer(std::shared_ptr<models::Scene> scene, const std::string& address,
                           uint16_t port, bool master)
    : scene_{std::move(scene)}
    , master_{master}
    , tcp_server_(address, port)
    , fbs_builder_(MAX_MESSAGE_SIZE)
    , read_buffer_(MAX_MESSAGE_SIZE)
    , state_{State::wait} {
  network_thread_ = std::thread(&RewindServer::network_loop, this);
  network_thread_.detach();
}

RewindServer::~RewindServer() {
  state_ = State::closed;
  if (network_thread_.joinable()) {
    network_thread_.join();
  }
}

void RewindServer::stop() {
  state_ = State::closed;
}

void RewindServer::network_loop() {
  // It is ok to crash thread on init error
  tcp_server_.initialize();
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    // It is ok to crash thread on accept connection error
    LOG_INFO("Waiting new connection...");
    {
      std::lock_guard<Spinlock> lock(events_mutex_);
      events_.clear();
    }
    schema_version_ = 0;
    while (schema_version_ == 0) {
      schema_version_ = tcp_server_.accept_connection();
      if (schema_version_ == 0) {
        // TODO: handle app exit better
        return;
      }
      if (schema_version_ != MESSAGE_SCHEMA_VERSION &&
          schema_version_ != JSON_MESSAGE_SCHEMA_VERSION) {
        LOG_ERROR("Incorrect schema version, got %u, required %u (flatbuffers) or %u (json)",
                  schema_version_, MESSAGE_SCHEMA_VERSION, JSON_MESSAGE_SCHEMA_VERSION);
        tcp_server_.discard_connection();
        schema_version_ = 0;
      }
    }

    LOG_INFO("Schema version %u", schema_version_);
    if (schema_version_ == MESSAGE_SCHEMA_VERSION) {
      LOG_INFO("Using Flatbuffers serialization");
    } else {
      LOG_INFO("Using JSON serialization");
    }

    State expected = State::wait;
    if (state_.compare_exchange_strong(expected, State::established)) {
      reset();
      while (state_.load(std::memory_order_relaxed) == State::established) {
        try {
          uint32_t bytes = tcp_server_.read_msg(read_buffer_.data(), read_buffer_.size());

          if (schema_version_ == MESSAGE_SCHEMA_VERSION) {
            auto msg = fbs::GetRewindMessage(read_buffer_.data());
            handle_message(msg);
          } else {
            std::string json_message(read_buffer_.begin(), read_buffer_.begin() + bytes);
            rapidjson::Document doc;
            if (doc.Parse(json_message.c_str()).HasParseError()) {
              LOG_ERROR("JSON parse error: %s %s", rapidjson::GetParseError_En(doc.GetParseError()),
                        json_message.c_str());
            } else {
              handle_message(doc);
            }
          }
        } catch (std::exception const& e) {
          LOG_ERROR("Error reading message: %s", e.what());
          state_ = State::wait;
        } catch (...) {
          LOG_ERROR("Unknown error reading message");
          state_ = State::wait;
        }
      }
    }
  }
}

void RewindServer::reset() {
  if (master_) {
    scene_->frames.clear();
  }
  layer_id_ = DEFAULT_LAYER;
}

void RewindServer::handle_message(const fbs::RewindMessage* message) {
  auto [perma_frame, frame] = scene_->frames.get_working_frame();
  std::shared_ptr<models::Frame> draw_frame = use_permanent_ ? perma_frame : frame;

  switch (message->command_type()) {
    case fbs::Command_Circle: {
      LOG_V8("FlatBuffersHandler::CIRCLE");
      auto circle = message->command_as_Circle();
      if (circle->radius() <= 0.0) {
        throw ParsingError{"Circle radius should be positive, got " +
                           std::to_string(circle->radius())};
      }
      std::lock_guard lock(*draw_frame);
      if (circle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_).add_stencil_circle(
            {circle->center()->x(), circle->center()->y()}, circle->radius());
      } else {
        draw_frame->layer_primitives(layer_id_).add_circle(
            {circle->center()->x(), circle->center()->y()}, circle->radius(),
            circle->color()->value(), circle->color()->fill());
      }
      break;
    }
    case fbs::Command_CircleSegment: {
      LOG_V8("FlatBuffersHandler::CIRCLE_SEGMENT");
      auto segment = message->command_as_CircleSegment();
      if (segment->radius() <= 0.0) {
        throw ParsingError{"Circle segment radius should be positive, got " +
                           std::to_string(segment->radius())};
      }
      std::lock_guard lock(*draw_frame);
      if (segment->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_).add_stencil_segment(
            {segment->center()->x(), segment->center()->y()}, segment->radius(),
            segment->start_angle(), segment->end_angle());
      } else {
        draw_frame->layer_primitives(layer_id_).add_segment(
            {segment->center()->x(), segment->center()->y()}, segment->radius(),
            segment->start_angle(), segment->end_angle(), segment->color()->value(),
            segment->color()->fill());
      }
      break;
    }
    case fbs::Command_Arc: {
      LOG_V8("FlatBuffersHandler::ARC");
      auto arc = message->command_as_Arc();
      if (arc->radius() <= 0.0) {
        throw ParsingError{"Arc radius should be positive, got " + std::to_string(arc->radius())};
      }
      std::lock_guard lock(*draw_frame);
      if (arc->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_).add_stencil_arc(
            {arc->center()->x(), arc->center()->y()}, arc->radius(), arc->start_angle(),
            arc->end_angle());
      } else {
        draw_frame->layer_primitives(layer_id_).add_arc(
            {arc->center()->x(), arc->center()->y()}, arc->radius(), arc->start_angle(),
            arc->end_angle(), arc->color()->value(), arc->color()->fill());
      }
      break;
    }
    case fbs::Command_Triangle: {
      LOG_V8("FlatBuffersHandler::TRIANGLE");
      auto triangle = message->command_as_Triangle();
      if (triangle->points()->size() != 3) {
        throw ParsingError{"Triangle expect exactly 3 points, got " +
                           std::to_string(triangle->points()->size())};
      }
      std::vector<glm::vec2> points;
      for (auto p : *triangle->points()) {
        points.emplace_back(p->x(), p->y());
      }
      std::lock_guard lock(*draw_frame);
      if (triangle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_).add_stencil_triangle(points[0], points[1],
                                                                      points[2]);
      } else {
        // TODO: support for gradient colors
        draw_frame->layer_primitives(layer_id_).add_triangle(
            points[0], points[1], points[2], triangle->color()->value(), triangle->color()->fill());
      }
      break;
    }
    case fbs::Command_Polyline: {
      LOG_V8("FlatBuffersHandler::POLYLINE");
      auto polyline = message->command_as_Polyline();
      if (polyline->points()->size() < 2) {
        throw ParsingError{"Polyline expect exactly 2 or more points, got " +
                           std::to_string(polyline->points()->size())};
      }
      if (polyline->color() == nullptr) {
        throw ParsingError{"Polyline is not supported mask for now"};
      }
      if (polyline->color()->fill()) {
        LOG_ERROR("Polyline is not supporting fill for now. Parameter ignored.");
      }
      std::vector<glm::vec2> points;
      for (auto p : *polyline->points()) {
        points.emplace_back(p->x(), p->y());
      }
      std::lock_guard lock(*draw_frame);
      draw_frame->layer_primitives(layer_id_).add_polyline(points, polyline->color()->value());
      break;
    }
    case fbs::Command_Rectangle: {
      LOG_V8("FlatBuffersHandler::RECTANGLE");
      auto rectangle = message->command_as_Rectangle();
      if (rectangle->size()->x() <= 0.0) {
        throw ParsingError{"Rectangle width should be positive, got " +
                           std::to_string(rectangle->size()->x())};
      }
      if (rectangle->size()->y() <= 0.0) {
        throw ParsingError{"Rectangle height should be positive, got " +
                           std::to_string(rectangle->size()->y())};
      }
      glm::vec2 top_left{rectangle->position()->x(), rectangle->position()->y()};
      glm::vec2 bottom_right{top_left.x + rectangle->size()->x(),
                             top_left.y + rectangle->size()->y()};
      fix_AABB(top_left, bottom_right);
      std::lock_guard lock(*draw_frame);
      if (rectangle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_).add_stencil_rectangle(top_left, bottom_right);
      } else {
        draw_frame->layer_primitives(layer_id_).add_rectangle(
            top_left, bottom_right, rectangle->color()->value(), rectangle->color()->fill());
      }
      break;
    }
    case fbs::Command_Popup: {
      LOG_V8("FlatBuffersHandler::POPUP");
      auto popup = message->command_as_Popup();
      if (popup->area_size()->x() <= 0.0) {
        throw ParsingError{"Popup area width should be positive, got " +
                           std::to_string(popup->area_size()->x())};
      }
      if (popup->area_size()->y() <= 0.0) {
        throw ParsingError{"Popup area height should be positive, got " +
                           std::to_string(popup->area_size()->y())};
      }

      frame->add_box_popup(layer_id_,
                           {popup->area_position()->x() + 0.5 * popup->area_size()->x(),
                            popup->area_position()->y() + 0.5 * popup->area_size()->y()},
                           {popup->area_size()->x(), popup->area_size()->y()},
                           popup->text()->str());
      break;
    }
    case fbs::Command_PopupRound: {
      LOG_V8("FlatBuffersHandler::POPUP_ROUND");
      auto popup = message->command_as_PopupRound();
      if (popup->area_radius() <= 0.0) {
        throw ParsingError{"Popup area radius should be positive, got " +
                           std::to_string(popup->area_radius())};
      }

      frame->add_round_popup(layer_id_, {popup->area_center()->x(), popup->area_center()->y()},
                             popup->area_radius(), popup->text()->str());
      break;
    }
    case fbs::Command_CameraView: {
      LOG_V8("FlatBuffersHandler::CAMERA_VIEW");
      auto cam_view_msg = message->command_as_CameraView();
      models::CameraView cam_view{
          {cam_view_msg->position()->x(), cam_view_msg->position()->y()},
          {cam_view_msg->view_radius() * 2.0f, cam_view_msg->view_radius() * 2.0f}};

      frame->add_camera_view(cam_view_msg->name()->str(), cam_view);
      break;
    }
    case fbs::Command_Options: {
      LOG_V8("FlatBuffersHandler::OPTIONS");
      auto options = message->command_as_Options();
      if (options->map() != nullptr) {
        LOG_V8("FlatBuffersHandler::OPTIONS->MAP");
        if (master_) {
          auto map = options->map();
          if (map->size()->x() <= 0.0) {
            throw ParsingError{"Map width should be positive, got " +
                               std::to_string(map->size()->x())};
          }
          if (map->size()->y() <= 0.0) {
            throw ParsingError{"Map height should be positive, got " +
                               std::to_string(map->size()->y())};
          }
          reset();
          scene_->set_canvas_config({map->position()->x(), map->position()->y()},
                                    {map->size()->x(), map->size()->y()},
                                    {map->x_grid(), map->y_grid()});
        }
      }
      if (options->layer() != nullptr) {
        LOG_V8("FlatBuffersHandler::OPTIONS->LAYER");
        auto layer = options->layer();
        use_permanent_ = layer->use_permanent_frame();
        layer_id_ = layer->id();
      }
      break;
    }
    case fbs::Command_LogText: {
      LOG_V8("FlatBuffersHandler::LOG_TEXT");
      auto logText = message->command_as_LogText();

      frame->add_user_text(logText->text()->str());
      break;
    }
    case fbs::Command_EndFrame: {
      LOG_V8("FlatBuffersHandler::END_FRAME");
      if (master_) {
        scene_->frames.commit_frame();
      }
      break;
    }
    case fbs::Command_Tiles: {
      LOG_V8("FlatBuffersHandler::COLOR_FIELD");
      auto tiles = message->command_as_Tiles();
      glm::vec2 cell{tiles->cell_size()->x(), tiles->cell_size()->y()};
      glm::vec2 position{tiles->position()->x(), tiles->position()->y()};
      //      position -= 0.5f * cell;
      float start_x = position.x;
      auto row_size = tiles->row_size();
      if (row_size == 0) {
        throw ParsingError{"Tiles row size should be greater than 0"};
      }
      std::lock_guard lock(*draw_frame);
      auto& primitives = draw_frame->layer_primitives(layer_id_);
      size_t i = 0;
      for (auto color : *tiles->colors()) {
        if (color & 0xFF000000) {  // Ignore transparent
          primitives.add_rectangle(position, position + cell, color, true);
        }
        if (++i == row_size) {
          i = 0;
          position.y += cell.y;
          position.x = start_x;
        } else {
          position.x += cell.x;
        }
      }
      break;
    }
    case fbs::Command_Subscribe: {
      LOG_V8("FlatBuffersHandler::SUBSCRIBE");
      auto subscribe = message->command_as_Subscribe();
      std::lock_guard<Spinlock> lock(events_mutex_);

      if (events_.count(subscribe->key()) > 0) {
        LOG_WARN("OVERRIDE EVENT FOR KEY %d", subscribe->key());
      }
      if (subscribe->capture_mouse()) {
        events_[subscribe->key()] =
            std::make_unique<CursorEvent>(subscribe->name()->str(), subscribe->continuous(),
                                          subscribe->key(), subscribe->min_position_change());

      } else {
        events_[subscribe->key()] = std::make_unique<KeyEvent>(
            subscribe->name()->str(), subscribe->continuous(), subscribe->key());
      }
      break;
    }
    case fbs::Command_Unsubscribe: {
      LOG_V8("FlatBuffersHandler::UNSUBSCRIBE");
      auto unsubscribe = message->command_as_Unsubscribe();
      std::lock_guard<Spinlock> lock(events_mutex_);
      events_.erase(unsubscribe->key());
      break;
    }
    case fbs::Command_ReadEvents: {
      LOG_V8("FlatBuffersHandler::READ_EVENTS");
      std::lock_guard<Spinlock> lock(events_mutex_);
      fbs_builder_.Clear();
      std::vector<flatbuffers::Offset<rewind_viewer::fbs::RewindEvent>> event_offsets;
      for (const auto& [_, event] : events_) {
        if (event->is_triggered()) {
          event_offsets.push_back(event->serialize(fbs_builder_));
          event->reset_state();
        }
      }

      auto events_vector = fbs_builder_.CreateVector(event_offsets);
      auto event_list = rewind_viewer::fbs::CreateRewindEventList(fbs_builder_, events_vector);
      fbs_builder_.Finish(event_list);
      tcp_server_.send_msg(fbs_builder_.GetBufferPointer(), fbs_builder_.GetSize());
      break;
    }
    default: {
      LOG_ERROR("Unknown command type");
      break;
    }
  }
}

void RewindServer::handle_message(const rapidjson::Document& doc) {
  auto [perma_frame, frame] = scene_->frames.get_working_frame();
  std::shared_ptr<models::Frame> draw_frame = use_permanent_ ? perma_frame : frame;

  if (!doc.HasMember("cmd") || !doc["cmd"].IsObject()) {
    throw std::runtime_error("Invalid JSON message: missing 'cmd' object");
  }

  const auto& cmd_obj = doc["cmd"];
  if (!cmd_obj.HasMember("t") || !cmd_obj["t"].IsString()) {
    throw std::runtime_error("Invalid JSON message: missing 't' string");
  }

  std::string cmd_type = cmd_obj["t"].GetString();
  const auto& data_obj = cmd_obj["d"];

  if (cmd_type == "C") {
    LOG_V8("JSONHandler::CIRCLE");
    glm::vec2 center{data_obj["ct"]["x"].GetFloat(), data_obj["ct"]["y"].GetFloat()};
    float radius = data_obj["r"].GetFloat();
    if (radius <= 0.0f) {
      throw std::runtime_error("Circle radius should be positive, got " + std::to_string(radius));
    }
    std::lock_guard lock(*draw_frame);
    if (!data_obj.HasMember("c")) {
      draw_frame->layer_primitives(layer_id_).add_stencil_circle(center, radius);
    } else {
      auto color_obj = data_obj["c"].GetObject();
      uint32_t color = color_obj["v"].GetUint();
      bool fill = color_obj["f"].GetBool();
      draw_frame->layer_primitives(layer_id_).add_circle(center, radius, color, fill);
    }
  } else if (cmd_type == "CS") {
    LOG_V8("JSONHandler::CIRCLE_SEGMENT");
    glm::vec2 center{data_obj["ct"]["x"].GetFloat(), data_obj["ct"]["y"].GetFloat()};
    float radius = data_obj["r"].GetFloat();
    float start_angle = data_obj["sa"].GetFloat();
    float end_angle = data_obj["ea"].GetFloat();
    if (radius <= 0.0f) {
      throw std::runtime_error("Circle segment radius should be positive, got " +
                               std::to_string(radius));
    }
    std::lock_guard lock(*draw_frame);
    if (!data_obj.HasMember("c")) {
      draw_frame->layer_primitives(layer_id_).add_stencil_segment(center, radius, start_angle,
                                                                   end_angle);
    } else {
      auto color_obj = data_obj["c"].GetObject();
      uint32_t color = color_obj["v"].GetUint();
      bool fill = color_obj["f"].GetBool();
      draw_frame->layer_primitives(layer_id_).add_segment(center, radius, start_angle, end_angle,
                                                           color, fill);
    }
  } else if (cmd_type == "A") {
    LOG_V8("JSONHandler::ARC");
    glm::vec2 center{data_obj["ct"]["x"].GetFloat(), data_obj["ct"]["y"].GetFloat()};
    float radius = data_obj["r"].GetFloat();
    float start_angle = data_obj["sa"].GetFloat();
    float end_angle = data_obj["ea"].GetFloat();
    if (radius <= 0.0f) {
      throw std::runtime_error("Arc radius should be positive, got " + std::to_string(radius));
    }
    std::lock_guard lock(*draw_frame);
    if (!data_obj.HasMember("c")) {
      draw_frame->layer_primitives(layer_id_).add_stencil_arc(center, radius, start_angle,
                                                               end_angle);
    } else {
      auto color_obj = data_obj["c"].GetObject();
      uint32_t color = color_obj["v"].GetUint();
      bool fill = color_obj["f"].GetBool();
      draw_frame->layer_primitives(layer_id_).add_arc(center, radius, start_angle, end_angle,
                                                       color, fill);
    }
  } else if (cmd_type == "TR") {
    LOG_V8("JSONHandler::TRIANGLE");
    const auto& points_array = data_obj["pts"].GetArray();
    if (points_array.Size() != 3) {
      throw std::runtime_error("Triangle expects exactly 3 points, got " +
                               std::to_string(points_array.Size()));
    }
    std::vector<glm::vec2> points;
    for (const auto& p : points_array) {
      points.emplace_back(p["x"].GetFloat(), p["y"].GetFloat());
    }
    std::lock_guard lock(*draw_frame);
    if (!data_obj.HasMember("c")) {
      draw_frame->layer_primitives(layer_id_).add_stencil_triangle(points[0], points[1],
                                                                    points[2]);
    } else {
      auto color_obj = data_obj["c"].GetObject();
      uint32_t color = color_obj["v"].GetUint();
      bool fill = color_obj["f"].GetBool();
      draw_frame->layer_primitives(layer_id_).add_triangle(points[0], points[1], points[2], color,
                                                            fill);
    }
  } else if (cmd_type == "P") {
    LOG_V8("JSONHandler::POLYLINE");
    const auto& points_array = data_obj["pts"].GetArray();
    if (points_array.Size() < 2) {
      throw std::runtime_error("Polyline expects 2 or more points, got " +
                               std::to_string(points_array.Size()));
    }
    if (!data_obj.HasMember("c")) {
      throw std::runtime_error("Polyline is not supported as a mask for now");
    }
    auto color_obj = data_obj["c"].GetObject();
    uint32_t color = color_obj["v"].GetUint();
    bool fill = color_obj["f"].GetBool();
    if (fill) {
      LOG_ERROR("Polyline is not supporting fill for now. Parameter ignored.");
    }
    std::vector<glm::vec2> points;
    for (const auto& p : points_array) {
      points.emplace_back(p["x"].GetFloat(), p["y"].GetFloat());
    }
    std::lock_guard lock(*draw_frame);
    draw_frame->layer_primitives(layer_id_).add_polyline(points, color);
  } else if (cmd_type == "R") {
    LOG_V8("JSONHandler::RECTANGLE");
    glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
    glm::vec2 size{data_obj["s"]["x"].GetFloat(), data_obj["s"]["y"].GetFloat()};
    if (size.x <= 0.0f) {
      throw std::runtime_error("Rectangle width should be positive, got " + std::to_string(size.x));
    }
    if (size.y <= 0.0f) {
      throw std::runtime_error("Rectangle height should be positive, got " +
                               std::to_string(size.y));
    }
    glm::vec2 top_left = position;
    glm::vec2 bottom_right{top_left.x + size.x, top_left.y + size.y};
    fix_AABB(top_left, bottom_right);
    std::lock_guard lock(*draw_frame);
    if (!data_obj.HasMember("c")) {
      draw_frame->layer_primitives(layer_id_).add_stencil_rectangle(top_left, bottom_right);
    } else {
      auto color_obj = data_obj["c"].GetObject();
      uint32_t color = color_obj["v"].GetUint();
      bool fill = color_obj["f"].GetBool();
      draw_frame->layer_primitives(layer_id_).add_rectangle(top_left, bottom_right, color, fill);
    }
  } else if (cmd_type == "PP") {
    LOG_V8("JSONHandler::POPUP");
    glm::vec2 area_position{data_obj["ap"]["x"].GetFloat(), data_obj["ap"]["y"].GetFloat()};
    glm::vec2 area_size{data_obj["as"]["x"].GetFloat(), data_obj["as"]["y"].GetFloat()};
    if (area_size.x <= 0.0f) {
      throw std::runtime_error("Popup area width should be positive, got " +
                               std::to_string(area_size.x));
    }
    if (area_size.y <= 0.0f) {
      throw std::runtime_error("Popup area height should be positive, got " +
                               std::to_string(area_size.y));
    }
    std::string text = data_obj["t"].GetString();

    frame->add_box_popup(layer_id_, area_position + 0.5f * area_size, area_size, text);
  } else if (cmd_type == "PR") {
    LOG_V8("JSONHandler::POPUP_ROUND");
    glm::vec2 area_center{data_obj["ac"]["x"].GetFloat(), data_obj["ac"]["y"].GetFloat()};
    float area_radius = data_obj["r"].GetFloat();
    if (area_radius <= 0.0f) {
      throw std::runtime_error("Popup area radius should be positive, got " +
                               std::to_string(area_radius));
    }
    std::string text = data_obj["t"].GetString();

    frame->add_round_popup(layer_id_, area_center, area_radius, text);
  } else if (cmd_type == "CV") {
    LOG_V8("JSONHandler::CAMERA_VIEW");
    glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
    float view_radius = data_obj["vr"].GetFloat();
    models::CameraView cam_view{position, {view_radius * 2.0f, view_radius * 2.0f}};

    frame->add_camera_view(data_obj["n"].GetString(), cam_view);
  } else if (cmd_type == "O") {
    LOG_V8("JSONHandler::OPTIONS");
    if (data_obj.HasMember("m")) {
      LOG_V8("JSONHandler::OPTIONS->MAP");
      if (master_) {
        auto map_obj = data_obj["m"].GetObject();
        glm::vec2 position{map_obj["p"]["x"].GetFloat(), map_obj["p"]["y"].GetFloat()};
        glm::vec2 size{map_obj["s"]["x"].GetFloat(), map_obj["s"]["y"].GetFloat()};
        uint16_t x_grid = map_obj["xg"].GetUint();
        uint16_t y_grid = map_obj["yg"].GetUint();
        if (size.x <= 0.0f) {
          throw std::runtime_error("Map width should be positive, got " + std::to_string(size.x));
        }
        if (size.y <= 0.0f) {
          throw std::runtime_error("Map height should be positive, got " + std::to_string(size.y));
        }
        reset();
        scene_->set_canvas_config(position, size, {x_grid, y_grid});
      }
    }
    if (data_obj.HasMember("l")) {
      LOG_V8("JSONHandler::OPTIONS->LAYER");
      auto layer_obj = data_obj["l"].GetObject();
      use_permanent_ = layer_obj["upf"].GetBool();
      layer_id_ = layer_obj["i"].GetUint();
    }
  } else if (cmd_type == "LT") {
    LOG_V8("JSONHandler::LOG_TEXT");
    std::string text = data_obj["t"].GetString();

    frame->add_user_text(text);
  } else if (cmd_type == "EF") {
    LOG_V8("JSONHandler::END_FRAME");
    if (master_) {
      scene_->frames.commit_frame();
    }
  } else if (cmd_type == "CF") {
    LOG_V8("JSONHandler::COLOR_FIELD");
    glm::vec2 cell{data_obj["s"]["x"].GetFloat(), data_obj["s"]["y"].GetFloat()};
    glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
    float start_x = position.x;
    uint16_t row_size = data_obj["rs"].GetUint();
    if (row_size == 0) {
      throw std::runtime_error("Tiles row size should be greater than 0");
    }
    std::lock_guard lock(*draw_frame);
    auto& primitives = draw_frame->layer_primitives(layer_id_);
    size_t i = 0;
    for (const auto& color : data_obj["clrs"].GetArray()) {
      uint32_t color_value = color.GetUint();
      if (color_value & 0xFF000000) {  // Ignore transparent
        primitives.add_rectangle(position, position + cell, color_value, true);
      }
      if (++i == row_size) {
        i = 0;
        position.y += cell.y;
        position.x = start_x;
      } else {
        position.x += cell.x;
      }
    }
  } else if (cmd_type == "S") {
    LOG_V8("JSONHandler::SUBSCRIBE");
    std::lock_guard<Spinlock> lock(events_mutex_);
    bool continuous = data_obj.HasMember("c") && data_obj["c"].GetBool();
    char key = static_cast<char>(data_obj["k"].GetInt());
    std::string name = data_obj["n"].GetString();
    if (events_.count(key) > 0) {
      LOG_WARN("OVERRIDE EVENT FOR KEY %d", key);
    }
    if (data_obj["cm"].GetBool()) {
      if (data_obj.HasMember("mpc")) {
        double min_position_change = data_obj["mpc"].GetFloat();
        events_[key] = std::make_unique<CursorEvent>(name, continuous, key, min_position_change);
      } else {
        events_[key] = std::make_unique<CursorEvent>(name, continuous, key);
      }
    } else {
      events_[key] = std::make_unique<KeyEvent>(name, continuous, key);
    }
  } else if (cmd_type == "US") {
    LOG_V8("JSONHandler::UNSUBSCRIBE");
    std::lock_guard<Spinlock> lock(events_mutex_);
    events_.erase(static_cast<char>(data_obj["k"].GetInt()));
  } else if (cmd_type == "RE") {
    LOG_V8("JSONHandler::READ_EVENTS");
    std::lock_guard<Spinlock> lock(events_mutex_);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartObject();
    writer.Key("events");
    writer.StartArray();

    for (const auto& [_, event] : events_) {
      if (event->is_triggered()) {
        event->serialize(writer);
        event->reset_state();
      }
    }

    writer.EndArray();
    writer.EndObject();

    tcp_server_.send_msg(reinterpret_cast<const uint8_t*>(buffer.GetString()),
                         static_cast<uint32_t>(buffer.GetSize()));
  } else {
    LOG_ERROR("Unknown command type");
  }
}
}  // namespace rewind_viewer::net
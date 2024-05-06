#include "net/rewind_server.h"

#include <stdexcept>

#include "common/logger.h"

namespace {

void normalize(glm::vec2& min_corner, glm::vec2& max_corner) {
  if (min_corner.x > max_corner.x) {
    std::swap(min_corner.x, max_corner.x);
  }
  if (min_corner.y > max_corner.y) {
    std::swap(min_corner.y, max_corner.y);
  }
}

glm::vec4 convert_color(uint32_t value) {
  glm::vec4 result;
  result.r = static_cast<float>((value & 0xFF0000) >> 16) / 255.0f;
  result.g = static_cast<float>((value & 0x00FF00) >> 8) / 255.0f;
  result.b = static_cast<float>((value & 0x0000FF)) / 255.0f;

  uint8_t alpha = ((value & 0xFF000000) >> 24);
  if (alpha > 0) {
    result.a = static_cast<float>(alpha) / 255.0f;
  } else {
    result.a = 1.0f;
  }
  return result;
}

}  // namespace

namespace rewind_viewer::net {

RewindServer::RewindServer(std::shared_ptr<models::Scene> scene, const std::string& address,
                           uint16_t port, bool master)
    : scene_{std::move(scene)}
    , master_{master}
    , tcp_server_(address, port)
    , builder_(MAX_MESSAGE_SIZE)
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

RewindServer::State RewindServer::get_state() const {
  return state_.load(std::memory_order_relaxed);
}

uint16_t RewindServer::get_port() const {
  return tcp_server_.get_port();
}

void RewindServer::network_loop() {
  // It is ok to crash thread on init error
  tcp_server_.initialize();
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    // It is ok to crash thread on accept connection error
    LOG_INFO("Waiting new connection...");
    tcp_server_.accept_connection();
    State expected = State::wait;
    if (state_.compare_exchange_strong(expected, State::established)) {
      reset();
      while (state_.load(std::memory_order_relaxed) == State::established) {
        try {
          tcp_server_.read_msg(read_buffer_.data(), read_buffer_.size());
          auto msg = fbs::GetRewindMessage(read_buffer_.data());
          handle_message(msg);
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
    scene_->reset();
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
      if (circle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_)->add_stencil_circle(
            {circle->center()->x(), circle->center()->y()}, circle->radius());
      } else {
        draw_frame->layer_primitives(layer_id_)->add_circle(
            {circle->center()->x(), circle->center()->y()}, circle->radius(),
            convert_color(circle->color()->value()), circle->color()->fill());
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
      if (segment->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_)->add_stencil_segment(
            {segment->center()->x(), segment->center()->y()}, segment->radius(),
            segment->start_angle(), segment->end_angle());
      } else {
        draw_frame->layer_primitives(layer_id_)->add_segment(
            {segment->center()->x(), segment->center()->y()}, segment->radius(),
            segment->start_angle(), segment->end_angle(), convert_color(segment->color()->value()),
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
      if (arc->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_)->add_stencil_arc(
            {arc->center()->x(), arc->center()->y()}, arc->radius(), arc->start_angle(),
            arc->end_angle());
      } else {
        draw_frame->layer_primitives(layer_id_)->add_arc(
            {arc->center()->x(), arc->center()->y()}, arc->radius(), arc->start_angle(),
            arc->end_angle(), convert_color(arc->color()->value()), arc->color()->fill());
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
      if (triangle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_)->add_stencil_triangle(points[0], points[1],
                                                                      points[2]);
      } else {
        // TODO: support for gradient colors
        draw_frame->layer_primitives(layer_id_)->add_triangle(
            points[0], points[1], points[2], convert_color(triangle->color()->value()),
            triangle->color()->fill());
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
      draw_frame->layer_primitives(layer_id_)->add_polyline(
          points, convert_color(polyline->color()->value()));
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
      normalize(top_left, bottom_right);
      if (rectangle->color() == nullptr) {
        draw_frame->layer_primitives(layer_id_)->add_stencil_rectangle(top_left, bottom_right);
      } else {
        // TODO: support for gradient colors
        draw_frame->layer_primitives(layer_id_)->add_rectangle(
            top_left, bottom_right, convert_color(rectangle->color()->value()),
            rectangle->color()->fill());
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
          .position = {cam_view_msg->position()->x(), cam_view_msg->position()->y()},
          .viewport = {cam_view_msg->view_radius() * 2.0f, cam_view_msg->view_radius() * 2.0f}};
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
          if (map->width() <= 0.0) {
            throw ParsingError{"Map width should be positive, got " + std::to_string(map->width())};
          }
          if (map->height() <= 0.0) {
            throw ParsingError{"Map height should be positive, got " +
                               std::to_string(map->height())};
          }
          reset();
          scene_->set_canvas_config({map->width(), map->height()}, {map->x_grid(), map->y_grid()});
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
      auto primitives = draw_frame->layer_primitives(layer_id_);
      size_t i = 0;
      for (auto color : *tiles->colors()) {
        if (color & 0xFF000000) {  // Ignore transparent
          primitives->add_rectangle(position, position + cell, convert_color(color), true);
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
    default: {
      LOG_ERROR("Unknown command type");
      break;
    }
  }
}

}  // namespace rewind_viewer::net
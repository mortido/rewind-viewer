#pragma once

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/stringbuffer.h>

#include "common/logger.h"
#include "gateway/handlers/abstract.h"

namespace rewind_viewer::gateway {
class JsonMessageHandler : public MessageHandler {
 private:
  rapidjson::StringBuffer json_buffer_;

 public:
  JsonMessageHandler(models::SceneEditor& scene_editor, std::shared_ptr<Transport> transport)
      : MessageHandler(scene_editor, std::move(transport)) {}

  void handle_message(uint32_t size, LockDictionary<char, std::unique_ptr<Event>>& events,
                      LockDictionary<std::string, std::unique_ptr<Action>>& actions) override {
    rapidjson::Document doc;
    // Null-terminate the buffer at the correct position
    read_buffer_[size] = '\0';
    if (doc.Parse(reinterpret_cast<const char*>(read_buffer_.data())).HasParseError()) {
      LOG_ERROR("JSON parse error: %s", rapidjson::GetParseError_En(doc.GetParseError()));
      return;
    }

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
      if (!data_obj.HasMember("c")) {
        scene_editor_.add_stencil_circle(center, radius);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        scene_editor_.add_circle(center, radius, color, fill);
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
      if (!data_obj.HasMember("c")) {
        scene_editor_.add_stencil_segment(center, radius, start_angle, end_angle);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        scene_editor_.add_segment(center, radius, start_angle, end_angle, color, fill);
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
      if (!data_obj.HasMember("c")) {
        scene_editor_.add_stencil_arc(center, radius, start_angle, end_angle);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        scene_editor_.add_arc(center, radius, start_angle, end_angle, color, fill);
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
      if (!data_obj.HasMember("c")) {
        scene_editor_.add_stencil_triangle(points[0], points[1], points[2]);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        scene_editor_.add_triangle(points[0], points[1], points[2], color, fill);
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
      scene_editor_.add_polyline(points, color);
    } else if (cmd_type == "R") {
      LOG_V8("JSONHandler::RECTANGLE");
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      glm::vec2 size{data_obj["s"]["x"].GetFloat(), data_obj["s"]["y"].GetFloat()};
      if (size.x <= 0.0f) {
        throw std::runtime_error("Rectangle width should be positive, got " +
                                 std::to_string(size.x));
      }
      if (size.y <= 0.0f) {
        throw std::runtime_error("Rectangle height should be positive, got " +
                                 std::to_string(size.y));
      }
      glm::vec2 top_left = position;
      glm::vec2 bottom_right{top_left.x + size.x, top_left.y + size.y};
      normalize_AABB(top_left, bottom_right);
      if (!data_obj.HasMember("c")) {
        scene_editor_.add_stencil_rectangle(top_left, bottom_right);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        scene_editor_.add_rectangle(top_left, bottom_right, color, fill);
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

      scene_editor_.add_box_popup(area_position + 0.5f * area_size, area_size, text);
    } else if (cmd_type == "PR") {
      LOG_V8("JSONHandler::POPUP_ROUND");
      glm::vec2 area_center{data_obj["ac"]["x"].GetFloat(), data_obj["ac"]["y"].GetFloat()};
      float area_radius = data_obj["r"].GetFloat();
      if (area_radius <= 0.0f) {
        throw std::runtime_error("Popup area radius should be positive, got " +
                                 std::to_string(area_radius));
      }
      std::string text = data_obj["t"].GetString();

      scene_editor_.add_round_popup(area_center, area_radius, text);
    } else if (cmd_type == "CV") {
      LOG_V8("JSONHandler::CAMERA_VIEW");
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      float view_radius = data_obj["vr"].GetFloat();
      models::CameraView cam_view{position, {view_radius * 2.0f, view_radius * 2.0f}};

      scene_editor_.add_camera_view(data_obj["n"].GetString(), cam_view);
    } else if (cmd_type == "O") {
      LOG_V8("JSONHandler::OPTIONS");
      if (data_obj.HasMember("m")) {
        LOG_V8("JSONHandler::OPTIONS->MAP");

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
        scene_editor_.set_map(position, size, {x_grid, y_grid});
      }
      if (data_obj.HasMember("l")) {
        LOG_V8("JSONHandler::OPTIONS->LAYER");
        auto layer_obj = data_obj["l"].GetObject();
        uint32_t layer_id = layer_obj["i"].GetUint();
        bool use_permanent_frame = layer_obj["upf"].GetBool();

        // Parse origin if present
        models::CameraOrigin origin = models::CameraOrigin::game;
        if (layer_obj.HasMember("o")) {
          origin = static_cast<models::CameraOrigin>(layer_obj["o"].GetUint());
        }

        scene_editor_.set_layer(layer_id, use_permanent_frame, origin);

        // Set layer name if present
        if (layer_obj.HasMember("n")) {
          scene_editor_.set_layer_name(layer_id, layer_obj["n"].GetString(), use_permanent_frame);
        }
      }
    } else if (cmd_type == "LT") {
      LOG_V8("JSONHandler::LOG_TEXT");
      std::string text = data_obj["t"].GetString();
      scene_editor_.add_user_text(text);
    } else if (cmd_type == "EF") {
      LOG_V8("JSONHandler::END_FRAME");
      scene_editor_.finish_frame();
    } else if (cmd_type == "CF") {
      LOG_V8("JSONHandler::COLOR_FIELD");
      glm::vec2 cell{data_obj["s"]["x"].GetFloat(), data_obj["s"]["y"].GetFloat()};
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      float start_x = position.x;
      uint16_t row_size = data_obj["rs"].GetUint();
      if (row_size == 0) {
        throw std::runtime_error("Tiles row size should be greater than 0");
      }
      size_t i = 0;
      for (const auto& color : data_obj["clrs"].GetArray()) {
        uint32_t color_value = color.GetUint();
        if (color_value & 0xFF000000) {  // Ignore transparent
          scene_editor_.add_rectangle(position, position + cell, color_value, true);
        }
        if (++i == row_size) {
          i = 0;
          position.y += cell.y;
          position.x = start_x;
        } else {
          position.x += cell.x;
        }
      }
    } else if (cmd_type == "T") {
      LOG_V8("JSONHandler::TEXT");
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      float size = data_obj["s"].GetFloat();
      uint32_t color = data_obj["c"].GetUint();
      std::string text = data_obj["t"].GetString();
      if (size <= 0.0f) {
        throw std::runtime_error("Text font size should be positive, got " + std::to_string(size));
      }
      scene_editor_.add_text(position, size, color, text);
    } else if (cmd_type == "S") {
      LOG_V8("JSONHandler::SUBSCRIBE");
      bool continuous = data_obj.HasMember("c") && data_obj["c"].GetBool();
      char key = static_cast<char>(data_obj["k"].GetInt());
      std::string name = data_obj["n"].GetString();
      if (data_obj["cm"].GetBool()) {
        events.add(key, std::make_unique<CursorEvent>(key, name, continuous));
      } else {
        events.add(key, std::make_unique<KeyEvent>(key, name, continuous));
      }
    } else if (cmd_type == "US") {
      LOG_V8("JSONHandler::UNSUBSCRIBE");
      events.remove(static_cast<char>(data_obj["k"].GetInt()));
    } else if (cmd_type == "RE") {
      LOG_V8("JSONHandler::READ_EVENTS");
      json_buffer_.Clear();
      rapidjson::Writer<rapidjson::StringBuffer> writer(json_buffer_);
      writer.StartObject();
      writer.Key("events");
      writer.StartArray();
      events.iterate([&](auto, auto& event) {
        if (event->is_triggered()) {
          event->serialize(writer);
          event->reset_state();
        }
      });
      writer.EndArray();
      writer.Key("actions");
      writer.StartArray();
      actions.iterate([&](auto, auto& action) {
        if (action->is_triggered()) {
          action->serialize(writer);
          action->reset_state();
        }
      });
      writer.EndArray();
      writer.EndObject();
      transport_->send_msg(reinterpret_cast<const uint8_t*>(json_buffer_.GetString()),
                           static_cast<uint32_t>(json_buffer_.GetSize()));
    } else if (cmd_type == "SP") {
      LOG_V8("JSONHandler::START_PROTO");
      scene_editor_.start_proto();
    } else if (cmd_type == "EP") {
      LOG_V8("JSONHandler::END_PROTO");
      scene_editor_.end_proto();
    } else if (cmd_type == "DP") {
      LOG_V8("JSONHandler::DRAW_PROTO");
      size_t id = data_obj["id"].GetUint64();
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      float angle = data_obj.HasMember("a") ? data_obj["a"].GetFloat() : 0.0f;
      uint32_t color = data_obj.HasMember("c") ? data_obj["c"].GetUint() : 0;
      float scale = data_obj.HasMember("sc") ? data_obj["sc"].GetFloat() : 1.0f;
      scene_editor_.add_proto(id, position, angle, color, scale);
    } else if (cmd_type == "CA") {
      LOG_V8("JSONHandler::CREATE_ACTION");
      std::string action_name = data_obj["n"].GetString();
      std::string input_type = data_obj["it"].GetString();
      const auto& input_data = data_obj["id"];

      if (input_type == "bool") {
        bool default_value = input_data["dv"].GetBool();
        actions.add(action_name, std::make_unique<BoolInputAction>(action_name, default_value));
      } else if (input_type == "button") {
        actions.add(action_name, std::make_unique<ButtonAction>(action_name));
      } else if (input_type == "float") {
        float default_value = input_data["dv"].GetFloat();
        float min_value = input_data["min"].GetFloat();
        float max_value = input_data["max"].GetFloat();
        actions.add(action_name,
                    std::make_unique<FloatInputAction>(action_name, default_value, min_value,
                                                       max_value));
      } else if (input_type == "int") {
        int32_t default_value = input_data["dv"].GetInt();
        int32_t min_value = input_data["min"].GetInt();
        int32_t max_value = input_data["max"].GetInt();
        actions.add(action_name,
                    std::make_unique<IntInputAction>(action_name, default_value, min_value,
                                                     max_value));
      } else if (input_type == "select") {
        std::vector<std::string> options;
        for (const auto& option : input_data["opts"].GetArray()) {
          options.push_back(option.GetString());
        }
        uint16_t selected_option = input_data["so"].GetUint();
        actions.add(action_name,
                    std::make_unique<SelectInputAction>(action_name, std::move(options),
                                                        selected_option));
      } else if (input_type == "string") {
        std::string default_value = input_data["dv"].GetString();
        actions.add(action_name, std::make_unique<StringInputAction>(action_name, default_value));
      } else {
        LOG_ERROR("Unknown or Empty ActionType for CreateAction: %s", input_type.c_str());
      }
    } else if (cmd_type == "RA") {
      LOG_V8("JSONHandler::REMOVE_ACTION");
      std::string action_name = data_obj["n"].GetString();
      actions.remove(action_name);
    } else {
      LOG_ERROR("Unknown command type");
    }
  }
};
}  // namespace rewind_viewer::gateway
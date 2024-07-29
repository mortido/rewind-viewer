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
  void handle_message(const uint8_t* buffer, uint32_t, EventsCollection& events,
                      models::FrameEditor& frame_editor, Transport& transport) override {
    rapidjson::Document doc;
    // TODO: zero terminated?
    if (doc.Parse(reinterpret_cast<const char*>(buffer)).HasParseError()) {
      LOG_ERROR("JSON parse error: %s %s", rapidjson::GetParseError_En(doc.GetParseError()),
                reinterpret_cast<const char*>(buffer));
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
        frame_editor.add_stencil_circle(center, radius);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        frame_editor.add_circle(center, radius, color, fill);
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
        frame_editor.add_stencil_segment(center, radius, start_angle, end_angle);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        frame_editor.add_segment(center, radius, start_angle, end_angle, color, fill);
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
        frame_editor.add_stencil_arc(center, radius, start_angle, end_angle);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        frame_editor.add_arc(center, radius, start_angle, end_angle, color, fill);
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
        frame_editor.add_stencil_triangle(points[0], points[1], points[2]);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        frame_editor.add_triangle(points[0], points[1], points[2], color, fill);
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
      frame_editor.add_polyline(points, color);
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
        frame_editor.add_stencil_rectangle(top_left, bottom_right);
      } else {
        auto color_obj = data_obj["c"].GetObject();
        uint32_t color = color_obj["v"].GetUint();
        bool fill = color_obj["f"].GetBool();
        frame_editor.add_rectangle(top_left, bottom_right, color, fill);
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

      frame_editor.add_box_popup(area_position + 0.5f * area_size, area_size, text);
    } else if (cmd_type == "PR") {
      LOG_V8("JSONHandler::POPUP_ROUND");
      glm::vec2 area_center{data_obj["ac"]["x"].GetFloat(), data_obj["ac"]["y"].GetFloat()};
      float area_radius = data_obj["r"].GetFloat();
      if (area_radius <= 0.0f) {
        throw std::runtime_error("Popup area radius should be positive, got " +
                                 std::to_string(area_radius));
      }
      std::string text = data_obj["t"].GetString();

      frame_editor.add_round_popup(area_center, area_radius, text);
    } else if (cmd_type == "CV") {
      LOG_V8("JSONHandler::CAMERA_VIEW");
      glm::vec2 position{data_obj["p"]["x"].GetFloat(), data_obj["p"]["y"].GetFloat()};
      float view_radius = data_obj["vr"].GetFloat();
      models::CameraView cam_view{position, {view_radius * 2.0f, view_radius * 2.0f}};

      frame_editor.add_camera_view(data_obj["n"].GetString(), cam_view);
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
        frame_editor.set_map(position, size, {x_grid, y_grid});
      }
      if (data_obj.HasMember("l")) {
        LOG_V8("JSONHandler::OPTIONS->LAYER");
        auto layer_obj = data_obj["l"].GetObject();
        frame_editor.set_layer(layer_obj["i"].GetUint(), layer_obj["upf"].GetBool());
      }
    } else if (cmd_type == "LT") {
      LOG_V8("JSONHandler::LOG_TEXT");
      std::string text = data_obj["t"].GetString();
      frame_editor.add_user_text(text);
    } else if (cmd_type == "EF") {
      LOG_V8("JSONHandler::END_FRAME");
      frame_editor.finish_frame();
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
          frame_editor.add_rectangle(position, position + cell, color_value, true);
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
      bool continuous = data_obj.HasMember("c") && data_obj["c"].GetBool();
      char key = static_cast<char>(data_obj["k"].GetInt());
      std::string name = data_obj["n"].GetString();
      if (data_obj["cm"].GetBool()) {
        if (data_obj.HasMember("mpc")) {
          double min_position_change = data_obj["mpc"].GetFloat();
          events.add(key,
                     std::make_unique<CursorEvent>(name, continuous, key, min_position_change));
        } else {
          events.add(key, std::make_unique<CursorEvent>(name, continuous, key));
        }
      } else {
        events.add(key, std::make_unique<KeyEvent>(name, continuous, key));
      }
    } else if (cmd_type == "US") {
      LOG_V8("JSONHandler::UNSUBSCRIBE");
      events.remove(static_cast<char>(data_obj["k"].GetInt()));
    } else if (cmd_type == "RE") {
      LOG_V8("JSONHandler::READ_EVENTS");
      json_buffer_.Clear();
      rapidjson::Writer<rapidjson::StringBuffer> writer(json_buffer_);
      events.serialize(writer);
      events.reset();
      transport.send_msg(reinterpret_cast<const uint8_t*>(json_buffer_.GetString()),
                         static_cast<uint32_t>(json_buffer_.GetSize()));
    } else {
      LOG_ERROR("Unknown command type");
    }
  }
};
}  // namespace rewind_viewer::gateway
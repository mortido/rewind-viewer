#pragma once

#include <flatbuffers/flatbuffers.h>
#include <imgui/imgui.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <exception>
#include <string>
#include <unordered_map>
#include <utility>

#include "net/messages/rewind_event.fbs.h"

namespace rewind_viewer::net {

class Event {
 protected:
  std::string name_;
  bool continuous_ = false;
  ImGuiKey key_ = ImGuiKey_None;
  char key_char_;

  // Mapping from character to ImGuiKey
  ImGuiKey map_char_to_key(char key_char) {
    static const std::unordered_map<char, ImGuiKey> key_map = {
        {'a', ImGuiKey_A},           {'b', ImGuiKey_B},         {'c', ImGuiKey_C},
        {'d', ImGuiKey_D},           {'e', ImGuiKey_E},         {'f', ImGuiKey_F},
        {'g', ImGuiKey_G},           {'h', ImGuiKey_H},         {'i', ImGuiKey_I},
        {'j', ImGuiKey_J},           {'k', ImGuiKey_K},         {'l', ImGuiKey_L},
        {'m', ImGuiKey_M},           {'n', ImGuiKey_N},         {'o', ImGuiKey_O},
        {'p', ImGuiKey_P},           {'q', ImGuiKey_Q},         {'r', ImGuiKey_R},
        {'s', ImGuiKey_S},           {'t', ImGuiKey_T},         {'u', ImGuiKey_U},
        {'v', ImGuiKey_V},           {'w', ImGuiKey_W},         {'x', ImGuiKey_X},
        {'y', ImGuiKey_Y},           {'z', ImGuiKey_Z},         {'A', ImGuiKey_A},
        {'B', ImGuiKey_B},           {'C', ImGuiKey_C},         {'D', ImGuiKey_D},
        {'E', ImGuiKey_E},           {'F', ImGuiKey_F},         {'G', ImGuiKey_G},
        {'H', ImGuiKey_H},           {'I', ImGuiKey_I},         {'J', ImGuiKey_J},
        {'K', ImGuiKey_K},           {'L', ImGuiKey_L},         {'M', ImGuiKey_M},
        {'N', ImGuiKey_N},           {'O', ImGuiKey_O},         {'P', ImGuiKey_P},
        {'Q', ImGuiKey_Q},           {'R', ImGuiKey_R},         {'S', ImGuiKey_S},
        {'T', ImGuiKey_T},           {'U', ImGuiKey_U},         {'V', ImGuiKey_V},
        {'W', ImGuiKey_W},           {'X', ImGuiKey_X},         {'Y', ImGuiKey_Y},
        {'Z', ImGuiKey_Z},           {'<', ImGuiKey_MouseLeft}, {'>', ImGuiKey_MouseRight},
        {'|', ImGuiKey_MouseMiddle}, {'[', ImGuiKey_MouseX1},   {']', ImGuiKey_MouseX2}};

    auto it = key_map.find(key_char);
    if (it != key_map.end()) {
      return it->second;
    } else {
      throw std::runtime_error("Invalid key character: " + std::string(1, key_char));
    }
  }

 public:
  Event(std::string name, bool continuous, char key_char)
      : name_(std::move(name))
      , continuous_(continuous)
      , key_(map_char_to_key(key_char))
      , key_char_(key_char) {}

  virtual ~Event() = default;
  virtual bool capture(const glm::vec2& mouse_position) = 0;
  virtual void reset_state() = 0;
  virtual bool is_triggered() const = 0;

  virtual flatbuffers::Offset<rewind_viewer::fbs::RewindEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const = 0;
  virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
};

class CursorEvent : public Event {
  std::vector<std::vector<glm::vec2>> positions_;
  double min_position_change_;
  glm::vec2 prev_pos_{-100.0, -100.0};

 public:
  CursorEvent(std::string name, bool continuous, char key_char, double min_position_change = 1e-3)
      : Event(std::move(name), continuous, key_char), min_position_change_(min_position_change) {}

  bool capture(const glm::vec2& position) override {
    if (ImGui::IsKeyPressed(key_, !continuous_)) {
      positions_.emplace_back(std::vector<glm::vec2>{position});
      prev_pos_ = position;
      return true;
    } else if (continuous_ && ImGui::IsKeyDown(key_)) {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
      ImGui::BeginTooltip();
      ImGui::Text("Event: %s", name_.c_str());
      ImGui::EndTooltip();
      if (positions_.empty()) {
        positions_.emplace_back();
      }

      if (std::abs(prev_pos_.x - position.x) > min_position_change_ &&
          std::abs(prev_pos_.x - position.x) > min_position_change_) {
        positions_.back().emplace_back(position);
        prev_pos_ = position;
      }
      return true;
    }
    return false;
  }

  [[nodiscard]] bool is_triggered() const override {
    return !positions_.empty();
  }

  void reset_state() override {
    positions_.clear();
  }

  flatbuffers::Offset<rewind_viewer::fbs::RewindEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    std::vector<flatbuffers::Offset<rewind_viewer::fbs::MousePath>> paths;
    for (const auto& path : positions_) {
      std::vector<rewind_viewer::fbs::Vector2f> points;
      for (const auto& point : path) {
        points.emplace_back(point.x, point.y);
      }
      paths.emplace_back(
          rewind_viewer::fbs::CreateMousePath(builder, builder.CreateVectorOfStructs(points)));
    }

    return rewind_viewer::fbs::CreateRewindEvent(builder, key_char_, builder.CreateVector(paths));
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("k");
    writer.Uint(static_cast<uint8_t>(key_char_));
    writer.Key("mp");
    writer.StartArray();
    for (const auto& path : positions_) {
      writer.StartObject();
      writer.Key("p");
      writer.StartArray();
      for (const auto& point : path) {
        writer.StartObject();
        writer.Key("x");
        writer.Double(point.x);
        writer.Key("y");
        writer.Double(point.y);
        writer.EndObject();
      }
      writer.EndArray();
      writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
  }
};

class KeyEvent : public Event {
  bool triggered_ = false;

 public:
  KeyEvent(std::string name, bool continuous, char key_char)
      : Event(std::move(name), continuous, key_char) {}

  bool capture(const glm::vec2&) override {
    if (ImGui::IsKeyPressed(key_, true) || (continuous_ && ImGui::IsKeyDown(key_))) {
      triggered_ = true;
      return true;
    }
    return false;
  }

  [[nodiscard]] bool is_triggered() const override {
    return triggered_;
  }

  void reset_state() override {
    triggered_ = false;
  }

  flatbuffers::Offset<rewind_viewer::fbs::RewindEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    return rewind_viewer::fbs::CreateRewindEvent(builder, key_char_, 0);
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("k");
    writer.Uint(static_cast<uint8_t>(key_char_));
    writer.EndObject();
  }
};

}  // namespace rewind_viewer::net
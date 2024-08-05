#pragma once

#include <flatbuffers/flatbuffers.h>
#include <imgui/fontawesome.h>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <exception>
#include <map>
#include <mutex>
#include <string>
#include <utility>

#include "common/lock.h"
#include "gateway/messages/events.fbs.h"

namespace rewind_viewer::gateway {

class Action {
 protected:
  std::string name_;
  bool triggered_ = false;
  virtual void render_input() = 0;

 public:
  Action(std::string name) : name_(std::move(name)) {}

  virtual ~Action() = default;
  virtual void render() {
    static const ImVec4 triggered_color = ImVec4(0.741f, 0.38f, 0.229f, 1.0f);
    render_input();
    if (triggered_) {
      ImGui::SameLine();
      ImGui::TextColored(triggered_color, ICON_FA_ARROW_UP_FROM_BRACKET);
    }
  }
  virtual void reset_state() { triggered_ = false; }
  bool is_triggered() const { return triggered_; }

  virtual flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const = 0;
  virtual void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
};

class ButtonAction : public Action {
 public:
  ButtonAction(std::string name) : Action(std::move(name)) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_NONE, 0);
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.EndObject();
  }

 protected:
  void render_input() override {
    if (ImGui::Button(name_.c_str())) {
      triggered_ = true;
    }
  }
};

class IntInputAction : public Action {
 private:
  int value_;
  int min_value_;
  int max_value_;

 public:
  IntInputAction(std::string name, int default_value, int min_value, int max_value)
      : Action(std::move(name))
      , value_(default_value)
      , min_value_(min_value)
      , max_value_(max_value) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    auto value = rewind_viewer::fbs::CreateIntValue(builder, value_);
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_IntValue, value.Union());
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.Key("value");
    writer.Int(value_);
    writer.EndObject();
  }

 protected:
  void render_input() override {
    ImGui::PushItemWidth(140);
    if (min_value_ != max_value_) {
      if (ImGui::SliderInt(name_.c_str(), &value_, min_value_, max_value_, nullptr,
                           ImGuiSliderFlags_AlwaysClamp)) {
        triggered_ = true;
      }
    } else {
      if (ImGui::InputInt(name_.c_str(), &value_)) {
        triggered_ = true;
      }
    }
    ImGui::PopItemWidth();
  }
};

class FloatInputAction : public Action {
 private:
  float value_;
  float min_value_;
  float max_value_;

 public:
  FloatInputAction(std::string name, float default_value, float min_value, float max_value)
      : Action(std::move(name))
      , value_(default_value)
      , min_value_(min_value)
      , max_value_(max_value) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    auto value = rewind_viewer::fbs::CreateFloatValue(builder, value_);
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_FloatValue, value.Union());
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.Key("value");
    writer.Double(value_);
    writer.EndObject();
  }

 protected:
  void render_input() override {
    ImGui::PushItemWidth(140);
    if (min_value_ != max_value_) {
      if (ImGui::SliderFloat(name_.c_str(), &value_, min_value_, max_value_, nullptr,
                             ImGuiSliderFlags_AlwaysClamp)) {
        triggered_ = true;
      }
    } else {
      if (ImGui::InputFloat(name_.c_str(), &value_)) {
        triggered_ = true;
      }
    }
  }
};

class SelectInputAction : public Action {
 private:
  size_t selected_option_;
  std::vector<std::string> options_;

 public:
  SelectInputAction(std::string name, std::vector<std::string> options, int selected_option)
      : Action(std::move(name)), selected_option_(selected_option), options_(std::move(options)) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    auto value = rewind_viewer::fbs::CreateStringValue(
        builder, builder.CreateString(options_[selected_option_]));
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_StringValue, value.Union());
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.Key("value");
    writer.String(options_[selected_option_].c_str());
    writer.EndObject();
  }

 protected:
  void render_input() override {
    ImGui::PushItemWidth(140);
    if (ImGui::BeginCombo(name_.c_str(), options_[selected_option_].c_str())) {
      for (size_t i = 0; i < options_.size(); i++) {
        bool is_selected = (selected_option_ == i);
        if (ImGui::Selectable(options_[i].c_str(), is_selected)) {
          selected_option_ = i;
          triggered_ = true;
        }
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  }
};

class StringInputAction : public Action {
 private:
  std::string value_;

 public:
  StringInputAction(std::string name, std::string default_value)
      : Action(std::move(name)), value_(std::move(default_value)) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    auto value = rewind_viewer::fbs::CreateStringValue(builder, builder.CreateString(value_));
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_StringValue, value.Union());
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.Key("value");
    writer.String(value_.c_str());
    writer.EndObject();
  }

 protected:
  void render_input() override {
    ImGui::PushItemWidth(140);
    if (ImGui::InputText(name_.c_str(), &value_)) {
      triggered_ = true;
    }
  }
};

class BoolInputAction : public Action {
 private:
  bool value_;

 public:
  BoolInputAction(std::string name, bool default_value)
      : Action(std::move(name)), value_(default_value) {}

  flatbuffers::Offset<rewind_viewer::fbs::ActionEvent> serialize(
      flatbuffers::FlatBufferBuilder& builder) const override {
    auto name = builder.CreateString(name_);
    auto value = rewind_viewer::fbs::CreateBoolValue(builder, value_);
    return rewind_viewer::fbs::CreateActionEvent(
        builder, name, rewind_viewer::fbs::ActionValue::ActionValue_BoolValue, value.Union());
  }

  void serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override {
    writer.StartObject();
    writer.Key("name");
    writer.String(name_.c_str());
    writer.Key("value");
    writer.Bool(value_);
    writer.EndObject();
  }

 protected:
  void render_input() override {
    if (ImGui::Checkbox(name_.c_str(), &value_)) {
      triggered_ = true;
    }
  }
};

}  // namespace rewind_viewer::gateway

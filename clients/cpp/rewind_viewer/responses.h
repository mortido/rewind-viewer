#pragma once

#include <string>
#include <vector>

namespace rewind_viewer {

template <typename Vec2T>
struct RewindEvent {
  char key;
  std::vector<std::vector<Vec2T>> mouse_paths;
};

class ActionResult {
 public:
  enum class Type { Int, Float, Bool, String };

  ActionResult(std::string name, Type type) : name_(std::move(name)), type_(type) {}

  const std::string& get_name() const { return name_; }

  int get_int_value() const {
    if (type_ != Type::Int) throw std::runtime_error("Action value is not of type int");
    return int_value_;
  }

  float get_float_value() const {
    if (type_ != Type::Float) throw std::runtime_error("Action value is not of type float");
    return float_value_;
  }

  bool get_bool_value() const {
    if (type_ != Type::Bool) throw std::runtime_error("Action value is not of type bool");
    return bool_value_;
  }

  const std::string& get_string_value() const {
    if (type_ != Type::String) throw std::runtime_error("Action value is not of type string");
    return string_value_;
  }

  void set_value(int value) {
    type_ = Type::Int;
    int_value_ = value;
  }

  void set_value(float value) {
    type_ = Type::Float;
    float_value_ = value;
  }

  void set_value(bool value) {
    type_ = Type::Bool;
    bool_value_ = value;
  }

  void set_value(const std::string& value) {
    type_ = Type::String;
    string_value_ = value;
  }

 private:
  std::string name_;
  Type type_;
  int int_value_;
  float float_value_;
  bool bool_value_;
  std::string string_value_;
};

template <typename Vec2T>
struct EventsResponse {
  std::vector<RewindEvent<Vec2T>> events;
  std::vector<ActionResult> actions;
};
}  // namespace rewind_viewer
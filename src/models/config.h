#pragma once

#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <rapidyaml/ryml.hpp>
#include <string>

#include "common/logger.h"

namespace rewind_viewer::models {

class YamlConfig {
 public:
  virtual void parse(ryml::ConstNodeRef node) = 0;
  virtual void serialize(ryml::NodeRef node) const noexcept = 0;
  virtual bool validate(std::vector<std::string> &errors) const noexcept = 0;
};

enum class UIStyle : int { light, dark, classic };

struct UIConfig : public YamlConfig {
  int windows_width = 1200;
  int windows_height = 800;
  uint16_t fast_skip_speed = 20;
  bool close_with_esc = false;
  bool update_unfocused = false;
  int imgui_theme_id = 0;
  bool show_status = true;
  bool show_utility = true;
  int utility_width = 300;
  bool buffered_mode = true;
  glm::vec3 canvas_background_color = {0.2, 0.3, 0.3};
  std::string icon_file = "resources/icon.png";
  std::string font_file = "resources/fonts/fa-regular-400.ttf";
  std::string font_file2 = "resources/fonts/fa-solid-900.ttf";
  UIStyle style{UIStyle::light};

  void parse(ryml::ConstNodeRef node) override {}
  void serialize(ryml::NodeRef node) const noexcept override {}
  bool validate(std::vector<std::string> &errors) const noexcept override {
    return true;
  }
};

struct CameraConfig : public YamlConfig {
  bool y_axis_up = true;
  float zoom_speed = 0.1;
  glm::vec2 position{512.0f, 512.0f};
  float scale = 1.0;

  void parse(ryml::ConstNodeRef node) override {}
  void serialize(ryml::NodeRef node) const noexcept override {}
  bool validate(std::vector<std::string> &errors) const noexcept override {
    return true;
  }
};

struct SceneConfig : public YamlConfig {
  constexpr static size_t LAYERS_COUNT = 10;
  constexpr static size_t DEFAULT_LAYER = 2;

  glm::vec2 size = {1024.0f, 1024.0f};
  glm::u16vec2 grid_cells = {32, 32};
  glm::vec4 grid_color = {0.3219f, 0.336f, 0.392f, 1.0f};
  glm::vec4 background_color = {0.757f, 0.856f, 0.882f, 1.0f};
  bool show_grid = true;
  bool show_background = true;
  std::array<bool, LAYERS_COUNT> enabled_layers = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  std::array<bool, LAYERS_COUNT> enabled_permanent_layers = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  std::string textures_dir = "resources/textures/";
  std::string shaders_dir = "resources/shaders/";

  CameraConfig camera;

  void parse(ryml::ConstNodeRef node) override {}
  void serialize(ryml::NodeRef node) const noexcept override {}
  bool validate(std::vector<std::string> &errors) const noexcept override {
    return true;
  }
};

struct NetworkConfig : public YamlConfig {
  constexpr static uint8_t MAX_SLAVE_CONNECTIONS = 3u;

  std::string host = "127.0.0.1";
  uint16_t start_port = 9111;
  uint8_t slave_connections = 1;

  void parse(ryml::ConstNodeRef node) override {}
  void serialize(ryml::NodeRef node) const noexcept override {}
  bool validate(std::vector<std::string> &errors) const noexcept override {
    return true;
  }
};

class Config {
 public:
  UIConfig ui;
  SceneConfig scene;
  NetworkConfig network;

  static Config &get_instance() {
    static Config instance;
    return instance;
  }

  ~Config() = default;
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  bool validate(std::vector<std::string> &errors) const noexcept {
    bool result = true;
    result = ui.validate(errors) && result;
    result = scene.validate(errors) && result;
    result = network.validate(errors) && result;
    return result;
  }

  void load_from_file(const std::string &filename) {
    LOG_INFO("Loading configuration from file %s", filename.c_str());
    std::ifstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file: " + filename);
    }
    std::string yaml_str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    ryml::Tree tree = ryml::parse_in_place(ryml::substr(&yaml_str[0], yaml_str.size()));
    ryml::NodeRef root = tree.rootref();

    if (root.has_child("ui")) {
      ui.parse(root["ui"]);
    }

    if (root.has_child("scene")) {
      scene.parse(root["scene"]);
    }

    if (root.has_child("network")) {
      network.parse(root["network"]);
    }
  }

  void save_to_file(const std::string &filename) const {
    LOG_INFO("Saving configuration to file %s", filename.c_str());
    ryml::Tree tree;
    ryml::NodeRef root = tree.rootref();
    root |= ryml::MAP;

    ui.serialize(root["ui"]);
    scene.serialize(root["scene"]);
    network.serialize(root["network"]);

    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file: " + filename);
    }
    file << tree;
    file.close();
  }

 private:
  Config() = default;
};

}  // namespace rewind_viewer::models
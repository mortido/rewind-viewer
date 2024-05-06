#pragma once

#include <array>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <rapidyaml/ryml.hpp>
#include <string>

#include "common/logger.h"

namespace rewind_viewer::models {

class YamlConfig {
 public:
  virtual void parse(ryml::ConstNodeRef node) = 0;
  virtual void serialize(ryml::NodeRef node) const noexcept = 0;
  virtual bool validate(std::vector<std::string>& errors) const noexcept = 0;

 protected:
  static bool validate_color(const glm::vec4& color, const std::string& name,
                             std::vector<std::string>& errors) {
    if (color.r < 0.0f || color.r > 1.0f || color.g < 0.0f || color.g > 1.0f || color.b < 0.0f ||
        color.b > 1.0f || color.a < 0.0f || color.a > 1.0f) {
      errors.emplace_back(name + " color components must be between 0 and 1");
      return false;
    }
    return true;
  };

  static bool validate_color(const glm::vec3& color, const std::string& name,
                             std::vector<std::string>& errors) {
    if (color.r < 0.0f || color.r > 1.0f || color.g < 0.0f || color.g > 1.0f || color.b < 0.0f ||
        color.b > 1.0f) {
      errors.emplace_back(name + " color components must be between 0 and 1");
      return false;
    }
    return true;
  };

  static void write(ryml::NodeRef node, const glm::vec4& v) {
    node |= ryml::SEQ;
    node.append_child() << v.x;
    node.append_child() << v.y;
    node.append_child() << v.z;
    node.append_child() << v.w;
  }

  static void write(ryml::NodeRef node, const glm::vec3& v) {
    node |= ryml::SEQ;
    node.append_child() << v.x;
    node.append_child() << v.y;
    node.append_child() << v.z;
  }

  static void write(ryml::NodeRef node, const glm::u16vec2& v) {
    node |= ryml::SEQ;
    node.append_child() << v.x;
    node.append_child() << v.y;
  }

  static void write(ryml::NodeRef node, const glm::vec2& v) {
    node |= ryml::SEQ;
    node.append_child() << v.x;
    node.append_child() << v.y;
  }

  static void read(const std::string& name, ryml::ConstNodeRef node, glm::vec4& v) {
    if (!node.is_seq() || node.num_children() != 4) {
      throw std::runtime_error("Invalid YAML node '" + name +
                               "' for glm::vec4, expected a sequence of four elements");
    }
    node[0] >> v.x;
    node[1] >> v.y;
    node[2] >> v.z;
    node[3] >> v.w;
  }

  static void read(const std::string& name, ryml::ConstNodeRef node, glm::vec3& v) {
    if (!node.is_seq() || node.num_children() != 3) {
      throw std::runtime_error("Invalid YAML node '" + name +
                               "' for glm::vec3, expected a sequence of three elements");
    }
    node[0] >> v.x;
    node[1] >> v.y;
    node[2] >> v.z;
  }

  static void read(const std::string& name, ryml::ConstNodeRef node, glm::u16vec2& v) {
    if (!node.is_seq() || node.num_children() != 2) {
      throw std::runtime_error("Invalid YAML node '" + name +
                               "' for glm::u16vec2, expected a sequence of two elements");
    }
    node[0] >> v.x;
    node[1] >> v.y;
  }

  static void read(const std::string& name, ryml::ConstNodeRef node, glm::vec2& v) {
    if (!node.is_seq() || node.num_children() != 2) {
      throw std::runtime_error("Invalid YAML node '" + name +
                               "' for glm::vec2, expected a sequence of two elements");
    }
    node[0] >> v.x;
    node[1] >> v.y;
  }
};

enum class UIStyle : int { light, dark, classic };

struct UIConfig : public YamlConfig {
  int windows_width = 1200;
  int windows_height = 800;
  uint16_t fast_skip_speed = 20;
  bool close_with_esc = false;
  bool update_unfocused = false;
  int utility_width = 300;
  bool buffered_mode = true;
  glm::vec3 canvas_background_color = {0.2, 0.3, 0.3};
  std::string icon_file = "resources/icon.png";
  std::vector<std::string> font_files = {"resources/fonts/fa-regular-400.ttf",
                                         "resources/fonts/fa-solid-900.ttf"};
  UIStyle style{UIStyle::light};

  void parse(ryml::ConstNodeRef node) override {
    if (node.has_child("windows_width")) {
      node["windows_width"] >> windows_width;
    }
    if (node.has_child("windows_height")) {
      node["windows_height"] >> windows_height;
    }
    if (node.has_child("fast_skip_speed")) {
      node["fast_skip_speed"] >> fast_skip_speed;
    }
    if (node.has_child("close_with_esc")) {
      node["close_with_esc"] >> close_with_esc;
    }
    if (node.has_child("update_unfocused")) {
      node["update_unfocused"] >> update_unfocused;
    }
    if (node.has_child("buffered_mode")) {
      node["buffered_mode"] >> buffered_mode;
    }
    if (node.has_child("canvas_background_color")) {
      read("canvas_background_color", node["canvas_background_color"], canvas_background_color);
    }

    if (node.has_child("style")) {
      std::string style_str;
      node["style"] >> style_str;
      if (style_str == "light") {
        style = UIStyle::light;
      } else if (style_str == "dark") {
        style = UIStyle::dark;
      } else if (style_str == "classic") {
        style = UIStyle::classic;
      } else {
        throw std::runtime_error("Invalid style value: " + style_str);
      }
    }
  }
  void serialize(ryml::NodeRef node) const noexcept override {
    node |= ryml::MAP;
    node["windows_width"] << windows_width;
    node["windows_height"] << windows_height;
    node["fast_skip_speed"] << fast_skip_speed;
    node["close_with_esc"] << close_with_esc;
    node["update_unfocused"] << update_unfocused;
    node["buffered_mode"] << buffered_mode;
    write(node["canvas_background_color"], canvas_background_color);
    const char* style_str = "";
    switch (style) {
      case UIStyle::light: style_str = "light"; break;
      case UIStyle::dark: style_str = "dark"; break;
      case UIStyle::classic: style_str = "classic"; break;
      default: throw std::runtime_error("Unknown UIStyle enum value.");
    }
    node["style"] << style_str;
  }
  bool validate(std::vector<std::string>& errors) const noexcept override {
    bool is_valid = true;
    if (windows_width < 500 || windows_height < 500) {
      errors.emplace_back("Window dimensions must be more than 500x500");
      is_valid = false;
    }
    is_valid = validate_color(canvas_background_color, "UI:CanvasBackground", errors) && is_valid;
    if (fast_skip_speed <= 0) {
      errors.emplace_back("Fast skip speed must be a positive value");
      is_valid = false;
    }
    return is_valid;
  }
};

struct CameraConfig : public YamlConfig {
  bool y_axis_up = true;
  float zoom_speed = 0.1;  // negative for invert zoom
  glm::vec2 position{512.0f, 512.0f};
  float scale = 1.0;

  void parse(ryml::ConstNodeRef node) override {
    if (node.has_child("y_axis_up")) {
      node["y_axis_up"] >> y_axis_up;
    }
    if (node.has_child("zoom_speed")) {
      node["zoom_speed"] >> zoom_speed;
    }
    if (node.has_child("position")) {
      read("Camera:position", node["position"], position);
    }
    if (node.has_child("scale")) {
      node["scale"] >> scale;
    }
  }

  void serialize(ryml::NodeRef node) const noexcept override {
    node |= ryml::MAP;
    node["y_axis_up"] << y_axis_up;
    node["zoom_speed"] << zoom_speed;
    write(node["position"], position);
    node["scale"] << scale;
  }

  bool validate(std::vector<std::string>& errors) const noexcept override {
    bool is_valid = true;
    if (scale < 0.01f || scale > 100.0f) {
      errors.emplace_back("Camera scale must be between 0.01 and 100.0");
      is_valid = false;
    }
    return is_valid;
  }
};

struct SceneConfig : public YamlConfig {
  constexpr static size_t LAYERS_COUNT = 10;

  glm::vec2 size = {1024.0f, 1024.0f};
  glm::u16vec2 grid_cells = {32, 32};
  glm::vec4 grid_color = {0.3219f, 0.336f, 0.392f, 1.0f};
  glm::vec4 background_color = {0.757f, 0.856f, 0.882f, 1.0f};
  bool show_grid = true;
  bool show_background = true;
  bool show_game_coordinates = false;
  std::array<bool, LAYERS_COUNT> enabled_layers = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  std::array<bool, LAYERS_COUNT> enabled_permanent_layers = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
  std::string textures_dir = "resources/textures/";
  std::string shaders_dir = "resources/shaders/";

  CameraConfig camera;

  void parse(ryml::ConstNodeRef node) override {
    if (node.has_child("size")) {
      read("Scene:size", node["size"], size);
    }
    if (node.has_child("grid_cells")) {
      read("Scene:grid_cells", node["grid_cells"], grid_cells);
    }
    if (node.has_child("grid_color")) {
      read("Scene:grid_color", node["grid_color"], grid_color);
    }
    if (node.has_child("background_color")) {
      read("Scene:background_color", node["background_color"], background_color);
    }
    if (node.has_child("show_grid")) {
      node["show_grid"] >> show_grid;
    }
    if (node.has_child("show_background")) {
      node["show_background"] >> show_background;
    }
    if (node.has_child("show_game_coordinates")) {
      node["show_game_coordinates"] >> show_game_coordinates;
    }
    if (node.has_child("textures_dir")) {
      node["textures_dir"] >> textures_dir;
    }
    if (node.has_child("shaders_dir")) {
      node["shaders_dir"] >> shaders_dir;
    }
    if (node.has_child("camera")) {
      camera.parse(node["camera"]);
    }
  }

  void serialize(ryml::NodeRef node) const noexcept override {
    node |= ryml::MAP;
    write(node["size"], size);
    write(node["grid_cells"], grid_cells);
    write(node["grid_color"], grid_color);
    write(node["background_color"], background_color);
    node["show_grid"] << show_grid;
    node["show_background"] << show_background;
    node["show_game_coordinates"] << show_game_coordinates;
    node["textures_dir"] << textures_dir;
    node["shaders_dir"] << shaders_dir;
    camera.serialize(node["camera"]);
  }

  bool validate(std::vector<std::string>& errors) const noexcept override {
    bool is_valid = true;

    if (size.x <= 0.0f || size.y <= 0.0f) {
      errors.emplace_back("Scene size dimensions must be positive");
      is_valid = false;
    }
    if (grid_cells.x == 0 || grid_cells.y == 0) {
      errors.emplace_back("Scene grid cells must have non-zero dimensions");
      is_valid = false;
    }

    is_valid = validate_color(grid_color, "Scene:Grid", errors) && is_valid;
    is_valid = validate_color(background_color, "Scene:Background", errors) && is_valid;
    if (textures_dir.empty()) {
      errors.emplace_back("Textures directory should not be empty");
      is_valid = false;
    }
    if (shaders_dir.empty()) {
      errors.emplace_back("Shaders directory should not be empty");
      is_valid = false;
    }

    is_valid = camera.validate(errors) && is_valid;
    return is_valid;
  }
};

struct NetworkConfig : public YamlConfig {
  constexpr static uint16_t MAX_SLAVE_CONNECTIONS = 3u;

  std::string host = "127.0.0.1";
  uint16_t start_port = 9111;
  uint16_t slave_connections = 1;

  void parse(ryml::ConstNodeRef node) override {
    if (node.has_child("host")) {
      node["host"] >> host;
    }
    if (node.has_child("start_port")) {
      node["start_port"] >> start_port;
    }
    if (node.has_child("slave_connections")) {
      node["slave_connections"] >> slave_connections;
    }
  }

  void serialize(ryml::NodeRef node) const noexcept override {
    node |= ryml::MAP;
    node["host"] << host;
    node["start_port"] << start_port;
    node["slave_connections"] << slave_connections;
  }

  bool validate(std::vector<std::string>& errors) const noexcept override {
    bool is_valid = true;
    if (host.empty()) {
      errors.emplace_back("Host is empty");
      is_valid = false;
    }

    if (slave_connections > MAX_SLAVE_CONNECTIONS) {
      errors.emplace_back("Slave connections must less than " +
                          std::to_string(MAX_SLAVE_CONNECTIONS));
      is_valid = false;
    }
    return is_valid;
  }
};

class Config {
 public:
  const std::shared_ptr<UIConfig> ui;
  const std::shared_ptr<SceneConfig> scene;
  const std::shared_ptr<NetworkConfig> network;

  static Config& get_instance() {
    static Config instance;
    return instance;
  }

  ~Config() = default;
  Config(const Config&) = delete;
  Config& operator=(const Config&) = delete;

  bool validate(std::vector<std::string>& errors) const noexcept {
    bool result = true;
    result = ui->validate(errors) && result;
    result = scene->validate(errors) && result;
    result = network->validate(errors) && result;
    return result;
  }

  void load_from_file(const std::string& filename) {
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
      ui->parse(root["ui"]);
    }

    if (root.has_child("scene")) {
      scene->parse(root["scene"]);
    }

    if (root.has_child("network")) {
      network->parse(root["network"]);
    }
  }

  void save_to_file(const std::string& filename) const {
    LOG_INFO("Saving configuration to file %s", filename.c_str());
    ryml::Tree tree;
    ryml::NodeRef root = tree.rootref();
    root |= ryml::MAP;

    ui->serialize(root["ui"]);
    scene->serialize(root["scene"]);
    network->serialize(root["network"]);

    std::ofstream file(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Unable to open file: " + filename);
    }
    file << tree;
    file.close();
  }

 private:
  Config()
      : ui{std::make_shared<UIConfig>()}
      , scene{std::make_shared<SceneConfig>()}
      , network{std::make_shared<NetworkConfig>()} {};
};

}  // namespace rewind_viewer::models

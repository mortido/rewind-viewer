#pragma once

#include <memory>
#include <vector>

#include "models/camera.h"
#include "models/config.h"
#include "models/scene.h"
#include "net/rewind_server.h"

namespace rewind_viewer {

class RewindViewer {
 private:
  constexpr static float DEFAULT_FONT_SIZE = 13.0f;
  constexpr static float FONT_AWESOME_FONT_SIZE = 14.0f;

 public:
  RewindViewer(models::Config& config);
  ~RewindViewer() = default;

  void render();
  void stop();

 private:
  models::Config& config_;
  std::shared_ptr<models::Scene> scene_;
  std::vector<std::unique_ptr<net::RewindServer>> servers_;

  struct {
    float main_menu_height = 0.0f;
    bool show_status_overlay = true;
    bool show_playback_controls = true;
    float playback_controls_height = 0.0f;
    bool show_frame_info = true;
    float frame_info_width = 0.0f;

    bool show_shortcuts_help = false;
    bool show_mouse_pos_tooltip = true;

    bool developer_mode = true;
    bool show_ui_help = false;
    bool show_metrics = false;
    bool show_style_editor = false;

    bool autoplay = false;

    bool ignore_frame_camera_viewport = false;
    std::string selected_camera = "";
  } ui_state_;

  void main_menu_bar();
  void status_overlay();
  void playback_controls();
  void frame_info();
  void shortcuts_help();

  void viewport();

  void handle_keys(); // todo: move up

  void style_editor();
  void ui_help();
  void metrics();

  void setup_fonts();
  void setup_style();
};

}  // namespace rewind_viewer
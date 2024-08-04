
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gateway/client_gateway.h"

namespace rewind_viewer::ui {
struct RewindViewerState {
  size_t current_frame_idx = 0ul;
  float main_menu_height = 0.0f;
  float playback_controls_height = 0.0f;
  float frame_info_width = 0.0f;

  bool show_status_overlay = true;
  bool show_toolbox_panel = true;
  bool show_control_pad_window = false;
  bool show_playback_controls = true;
  bool show_shortcuts_help = false;

  bool developer_mode = false;
  bool show_ui_help = false;
  bool show_metrics = false;
  bool show_style_editor = false;

  bool autoplay = false;
  double last_frame_time = ImGui::GetTime();
  bool ignore_frame_camera_viewport = false;
  bool close_requested = false;
  std::string selected_camera;
};

}  // namespace rewind_viewer::ui
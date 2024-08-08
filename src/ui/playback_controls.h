
#pragma once

#include <imgui/imgui.h>

#include <string>

#include "models/config.h"
#include "models/scene.h"
#include "ui/state.h"

namespace rewind_viewer::ui {
class PlaybackControls {
 private:
  bool autoplay_ = false;
  double last_frame_time_ = ImGui::GetTime();

 public:
  void render(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene);

 private:
  void handle_inputs(const models::Config& config, models::Scene& scene);
};
}  // namespace rewind_viewer::ui
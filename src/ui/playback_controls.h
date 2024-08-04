
#pragma once

#include <string>
#include "models/config.h"
#include "models/scene.h"
#include "ui/state.h"

namespace rewind_viewer::ui {
class PlaybackControls {
 public:
  void render(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene);
  void handle_inputs(RewindViewerState& ui_state, const models::Config& config);
};
}
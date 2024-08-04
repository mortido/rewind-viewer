#pragma once

#include <memory>
#include <vector>

#include "models/camera.h"
#include "models/config.h"
#include "models/scene.h"
#include "ui/state.h"
#include "ui/main_menu.h"
#include "ui/status_overlay.h"
#include "ui/style_manager.h"
#include "ui/playback_controls.h"
#include "ui/toolbox_panel.h"
#include "ui/viewport.h"
#include "gateway/client_gateway.h"

namespace rewind_viewer {

class RewindViewer {
 public:
  RewindViewer(models::Config& config);
  ~RewindViewer() = default;

  bool update();
  void stop();
  bool close_requested() {
    return ui_state_.close_requested;
  }

 private:
  models::Config& config_;
  std::shared_ptr<models::Scene> scene_;
  std::vector<std::unique_ptr<gateway::ClientGateway>> gateways_;
  std::shared_ptr<models::UIFrame> current_frame_;
  ui::RewindViewerState ui_state_;

  ui::MainMenu main_menu_;
  ui::StatusOverlay status_overlay_;
  ui::PlaybackControls playback_controls_;
  ui::ToolboxPanel toolbox_panel_;
  ui::Viewport viewport_;

  ui::StyleManager style_manager_;


  void shortcuts_help();
  void handle_inputs();
  void style_editor();
  void ui_help();
  void metrics();
};

}  // namespace rewind_viewer
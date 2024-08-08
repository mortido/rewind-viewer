#pragma once

#include <memory>
#include <vector>

#include "models/camera.h"
#include "models/config.h"
#include "models/scene.h"
#include "ui/state.h"
#include "ui/main_menu.h"
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

 private:
  models::Config& config_;
  std::shared_ptr<models::Scene> scene_;
  std::vector<std::unique_ptr<gateway::ClientGateway>> gateways_;

  ui::StyleManager style_manager_;
  ui::RewindViewerState ui_state_;
  ui::MainMenu main_menu_;
  ui::PlaybackControls playback_controls_;
  ui::ToolboxPanel toolbox_panel_;
  ui::Viewport viewport_;

  void shortcuts_help();
  void handle_inputs();
  void style_editor();
  void ui_help();
  void metrics();
};

}  // namespace rewind_viewer
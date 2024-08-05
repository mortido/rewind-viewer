
#pragma once

#include <string>

#include "gateway/client_gateway.h"
#include "models/config.h"
#include "models/scene.h"
#include "ui/state.h"
#include "ui/style_manager.h"

namespace rewind_viewer::ui {

class MainMenu {
 public:
  void render(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene,
              std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways,
              StyleManager& style_manager);

 private:
  std::string open_file_dialog();
  std::string save_file_dialog();
};
}  // namespace rewind_viewer::ui
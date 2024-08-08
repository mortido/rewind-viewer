
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gateway/client_gateway.h"
#include "models/config.h"
#include "ui/state.h"
#include "ui/style_manager.h"

namespace rewind_viewer::ui {
class Viewport {
 public:
  void update(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene,
              std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways, const StyleManager& style_manager);
};

}  // namespace rewind_viewer::ui
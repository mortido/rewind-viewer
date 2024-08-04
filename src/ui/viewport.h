
#pragma once

#include <string>
#include "models/config.h"
#include "gateway/client_gateway.h"
#include "ui/state.h"
#include <vector>
#include <memory>

namespace rewind_viewer::ui {
class Viewport {
 public:
  void update(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene, std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways);
};

}
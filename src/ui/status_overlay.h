
#pragma once

#include <string>
#include "models/config.h"
#include "gateway/client_gateway.h"
#include <vector>
#include <memory>
#include "ui/state.h"

namespace rewind_viewer::ui {
class StatusOverlay {
 public:
  void render(RewindViewerState& ui_state, const models::Config& config, const std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways);
};

}
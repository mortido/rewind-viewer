
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gateway/client_gateway.h"
#include "models/config.h"
#include "ui/control_pad.h"
#include "ui/state.h"

namespace rewind_viewer::ui {
class ToolboxPanel {
 private:
  ControlPad control_pad_;

 public:
  void render(RewindViewerState& ui_state, const models::Config& config, models::Scene& scene,
              std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways);
};

}  // namespace rewind_viewer::ui
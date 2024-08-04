
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "gateway/client_gateway.h"
#include "ui/state.h"

namespace rewind_viewer::ui {
class ControlPad {
 public:
  void render(std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways);
};

}  // namespace rewind_viewer::ui
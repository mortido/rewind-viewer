#include "ui/control_pad.h"

#include <imgui/imgui.h>

namespace rewind_viewer::ui {
void ControlPad::render(std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways) {
  bool has_events = false;
  for (const auto& gateway : gateways) {
    auto& actions = gateway->get_actions();
    auto& events = gateway->get_events();
    if (!actions.empty() || !events.empty()) {
      ImGui::Text("CLIENT %s", gateway->get_name().c_str());
    }
    if (!actions.empty()) {
      has_events = true;
      actions.iterate([&](auto, auto& action) { action->render(); });
    }

    if (!events.empty()) {
      has_events = true;
      events.iterate([&](auto, auto& event) { event->render(); });
    }
  }

  if (!has_events) {
    ImGui::TextDisabled("No Events or Actions");
  }
}

}  // namespace rewind_viewer::ui
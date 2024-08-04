#include "ui/status_overlay.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>

namespace rewind_viewer::ui {

void StatusOverlay::render(RewindViewerState& ui_state, const models::Config& config,
                           const std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways) {
  if (!ui_state.show_status_overlay) {
    return;
  }

  ImGui::SetNextWindowPos(ImVec2(5.0f, ui_state.main_menu_height + 5.0f));
  const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
  ImGui::Begin("FPS Overlay", nullptr, flags);
  ImGui::PopStyleVar(2);

  ImGui::BeginGroup();
  ImGui::TextColored({1.0, 1.0, 0.0, 1.0}, "FPS %.1f", ImGui::GetIO().Framerate);
  ImGui::SameLine();
  ImGui::Text("[%.1f ms]", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::EndGroup();
  std::string strstatus;
  ImVec4 color;
  static const float intensity = 1.0;
  for (const auto& gateway : gateways) {
    switch (gateway->get_state()) {
      case gateway::ClientGateway::State::wait:
        strstatus = "WAITING";
        color = {intensity, intensity, 0.0, 1.0};
        break;
      case gateway::ClientGateway::State::established:
        strstatus = "CONNECTED";
        color = {0.0, intensity, 0.0, 1.0};
        break;
      case gateway::ClientGateway::State::aborting:
        strstatus = "ABORTING";
        color = {intensity, 0.0, 0.0, 1.0};
        break;
      case gateway::ClientGateway::State::closed:
        strstatus = "CLOSED";
        color = {intensity, 0.0, 0.0, 1.0};
        break;
    }

    ImGui::TextColored(color, ICON_FA_PLUG " %s: %s", gateway->get_name().c_str(),
                       strstatus.c_str());
  }
  const ImVec4 mode_color = {0.3f, 0.0f, 0.0f, 1.000f};
  if (!config.ui->buffered_mode) {
    ImGui::TextColored(mode_color, ICON_FA_CIRCLE_EXCLAMATION "Buffered mode disabled");
  }
  ImGui::End();
}

}  // namespace rewind_viewer::ui
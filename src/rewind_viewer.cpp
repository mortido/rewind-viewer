#include "rewind_viewer.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>

#include <algorithm>
#include <utility>

#include "gateway/transport/tcp_server.h"

namespace rewind_viewer {

RewindViewer::RewindViewer(models::Config &config)
    : config_{config}
    , scene_{std::make_shared<models::Scene>(config_.scene, config_.ui->buffered_mode)} {
  uint16_t port = config_.network->start_port;
  gateways_.emplace_back(std::make_unique<gateway::ClientGateway>(
      scene_, std::make_shared<gateway::TcpServer>(config_.network->host, port++), true));

  for (int i = 0; i < config_.network->slave_connections; i++) {
    gateways_.emplace_back(std::make_unique<gateway::ClientGateway>(
        scene_, std::make_shared<gateway::TcpServer>(config_.network->host, port++), false));
  }

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigWindowsResizeFromEdges = false;
  style_manager_.setup_fonts(config_.ui->font_files);
  style_manager_.setup_style(config_.ui->style);
}

bool RewindViewer::update() {
  // Handle keyboard before anything else
  handle_inputs();
  current_frame_ = scene_->get_frame(&ui_state_.current_frame_idx);

  main_menu_.render(ui_state_, config_, *scene_, gateways_, style_manager_);
  playback_controls_.handle_inputs(ui_state_, config_);
  playback_controls_.render(ui_state_, config_, *scene_);
    toolbox_panel_.render(ui_state_, config_, *scene_, gateways_);

  shortcuts_help();
  style_editor();
  ui_help();
  metrics();

  // Set viewport after all other windows size is known
  viewport_.update(ui_state_, config_, *scene_, gateways_);

  return !ui_state_.close_requested;
}

void RewindViewer::stop() {
  for (auto &s : gateways_) {
    s->stop();
  }
}

void RewindViewer::shortcuts_help() {
  if (!ui_state_.show_shortcuts_help) {
    return;
  }
  ImGui::Begin(ICON_FA_KEYBOARD " Controls help", &ui_state_.show_shortcuts_help,
               ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::BulletText("Mouse drag on map to move camera");
  ImGui::BulletText("Mouse wheel to zoom");
  ImGui::BulletText("Space - play/stop frame playback");
  ImGui::BulletText(ICON_FA_ARROW_LEFT ", " ICON_FA_ARROW_RIGHT
                                       " - manually change frames\n"
                                       "press with [Ctrl/Cmd] to change faster");
  ImGui::BulletText(ICON_FA_ARROW_UP ", " ICON_FA_ARROW_DOWN " - change replay FPS");
  if (config_.ui->close_with_esc) {
    ImGui::BulletText("Esc - close application");
  }
  ImGui::BulletText("[Ctrl/Cmd] + R - Clear frame data");
  ImGui::BulletText("1-0 - Toggle layers visibility");
  ImGui::End();
}

void RewindViewer::handle_inputs() {
  static const std::array<ImGuiKey, models::Frame::LAYERS_COUNT> layer_shortcuts = {
      ImGuiKey_1, ImGuiKey_2, ImGuiKey_3, ImGuiKey_4, ImGuiKey_5,
      ImGuiKey_6, ImGuiKey_7, ImGuiKey_8, ImGuiKey_9, ImGuiKey_0,
  };
  const auto &io = ImGui::GetIO();

  if (!io.WantTextInput) {
    if (config_.ui->close_with_esc && ImGui::IsKeyPressed(ImGuiKey_Escape)) {
      ui_state_.close_requested = true;
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey_D | ImGuiMod_Shortcut)) {
      ui_state_.developer_mode = true;
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey_R | ImGuiMod_Shortcut)) {
      scene_->clear();
    }

    // Layer toggle shortcuts
    for (size_t i = 0; i < layer_shortcuts.size(); ++i) {
      if (ImGui::IsKeyPressed(layer_shortcuts[i], false)) {
        config_.scene->enabled_layers[i] = !config_.scene->enabled_layers[i];
      }
    }
  }
}

void RewindViewer::style_editor() {
  if (ui_state_.show_style_editor) {
    ImGui::Begin("Style editor", &ui_state_.show_style_editor);
    ImGui::ShowStyleEditor();
    ImGui::End();
  }
}

void RewindViewer::ui_help() {
  if (ui_state_.show_ui_help) {
    ImGui::Begin(ICON_FA_CIRCLE_INFO " UI guide", &ui_state_.show_ui_help,
                 ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::ShowUserGuide();
    ImGui::End();
    ImGui::ShowDemoWindow();
  }
}

void RewindViewer::metrics() {
  if (ui_state_.show_metrics) {
    ImGui::ShowMetricsWindow(&ui_state_.show_metrics);
  }
}

}  // namespace rewind_viewer

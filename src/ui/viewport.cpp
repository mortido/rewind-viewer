
#include "ui/viewport.h"

#include <imgui/imgui.h>

namespace rewind_viewer::ui {
void Viewport::update(RewindViewerState& ui_state, const models::Config& config,
                      models::Scene& scene,
                      std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways) {
  ImGuiIO& io = ImGui::GetIO();
  // todo:
  auto current_frame_ = scene.get_frame(&ui_state.current_frame_idx);

  // Adjust viewport position and size to account for UI elements and padding
  ImVec2 viewport_pos = {1.0f * io.DisplayFramebufferScale.x,
                         (ui_state.playback_controls_height + 1.0f) * io.DisplayFramebufferScale.y};

  ImVec2 viewport_size = {
      (io.DisplaySize.x - ui_state.frame_info_width - 2.f) * io.DisplayFramebufferScale.x,
      (io.DisplaySize.y - ui_state.main_menu_height - ui_state.playback_controls_height - 2.f) *
          io.DisplayFramebufferScale.y};

  // Adjust the mouse position based on UI
  auto mouse_pos = io.MousePos;
  mouse_pos.x -= 1.0f;
  mouse_pos.y -= (ui_state.main_menu_height + 1.0f);
  mouse_pos.x *= io.DisplayFramebufferScale.x;
  mouse_pos.y *= io.DisplayFramebufferScale.y;

  // Set OpenGL viewport to the scaled and positioned area
  glViewport(static_cast<int>(viewport_pos.x), static_cast<int>(viewport_pos.y),
             static_cast<int>(viewport_size.x), static_cast<int>(viewport_size.y));

  // Correct the y-component of mouse coordinates to OpenGL space
  //  mouse_pos.y = viewport_size.y - mouse_pos.y;

  scene.camera.set_viewport_size(viewport_size);
  ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
  if (!io.WantCaptureMouse) {
    if (io.MouseWheel != 0.0f) {
      float zoom_factor = std::exp(-io.MouseWheel * config.scene->camera.zoom_speed);
      scene.camera.zoom(zoom_factor, mouse_pos);
      ui_state.ignore_frame_camera_viewport = true;
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      auto camera_move = io.MouseDelta;
      camera_move.x *= -io.DisplayFramebufferScale.x;
      camera_move.y *= -io.DisplayFramebufferScale.y;
      scene.camera.move(camera_move);

      ui_state.selected_camera.clear();
    }

    auto mouse_game_pos = scene.camera.screen_to_game(mouse_pos);
    if (config.scene->show_game_coordinates) {
      ImGui::BeginTooltip();
      ImGui::Text("(%.3f, %.3f)", mouse_game_pos.x, mouse_game_pos.y);
      ImGui::EndTooltip();
    }
    if (current_frame_) {
      auto popup_text =
          current_frame_->get_popup_text(mouse_game_pos, config.scene->enabled_layers);
      if (!popup_text.empty()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s", popup_text.c_str());
        ImGui::EndTooltip();
      }
    }

    if (!io.WantTextInput) {
      for (auto& server : gateways) {
        server->get_events().iterate([&](auto, auto& event) { event->capture(mouse_game_pos); });
      }
    }
  }

  if (!ui_state.selected_camera.empty()) {
    // TODO: clean up logic...
    if (current_frame_) {
      const auto& cameras = current_frame_->get_cameras();
      auto it = cameras.find(ui_state.selected_camera);
      if (it != cameras.end()) {
        scene.camera.set_view(it->second, ui_state.ignore_frame_camera_viewport);
      }
    }
  }
  scene.camera.update_projection();

  if (ui_state.autoplay) {
    double delta_time = ImGui::GetTime() - ui_state.last_frame_time;
    double frame_time = 1.0 / static_cast<double>(config.ui->replay_fps);
    if (delta_time >= frame_time) {
      ui_state.current_frame_idx++;
      ui_state.last_frame_time = ImGui::GetTime();
    }
  }
  scene.render(ui_state.current_frame_idx);

  // Cleanup opengl state
  glBindVertexArray(0);
  glUseProgram(0);
}

}  // namespace rewind_viewer::ui
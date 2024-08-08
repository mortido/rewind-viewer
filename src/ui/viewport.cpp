
#include "ui/viewport.h"

#include <imgui/imgui.h>

#include <array>

namespace rewind_viewer::ui {
void Viewport::update(RewindViewerState& ui_state, const models::Config& config,
                      models::Scene& scene,
                      std::vector<std::unique_ptr<gateway::ClientGateway>>& gateways,
                      const StyleManager& style_manager) {
  ImGuiIO& io = ImGui::GetIO();
  std::array<std::shared_ptr<models::Frame>, 2> frames = {scene.get_current_frame(),
                                                          scene.get_current_frame(true)};

  // Adjust viewport position and size to account for UI elements and padding
  glm::vec2 viewport_pos = {
      1.0f * io.DisplayFramebufferScale.x,
      (ui_state.playback_controls_height + 1.0f) * io.DisplayFramebufferScale.y};

  glm::vec2 viewport_size = {
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

  auto& camera = scene.get_camera();
  camera.set_viewport_size(viewport_size);
  ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
  if (!io.WantCaptureMouse) {
    auto mouse_game_pos = camera.screen_to_game(mouse_pos);
    if (io.MouseWheel != 0.0f) {
      float zoom_factor = std::exp(-io.MouseWheel * config.scene->camera.zoom_speed);
      camera.zoom(zoom_factor, mouse_game_pos);
      ui_state.ignore_camera_zoom = true;
    }

    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
      auto camera_move = io.MouseDelta;
      camera_move.x *= -io.DisplayFramebufferScale.x;
      camera_move.y *= -io.DisplayFramebufferScale.y;
      camera.move(camera_move);

      ui_state.selected_camera.clear();
    }

    if (config.scene->show_game_coordinates) {
      ImGui::BeginTooltip();
      ImGui::Text("(%.3f, %.3f)", mouse_game_pos.x, mouse_game_pos.y);
      ImGui::EndTooltip();
    }
    if (frames[0]) {
      for (size_t i = 0; i < 2; i++) {
        const auto& frame = frames[i];
        const auto& enabled =
            i == 0 ? config.scene->enabled_layers : config.scene->enabled_permanent_layers;
        for (size_t layer = 0; layer < models::Frame::LAYERS_COUNT; layer++) {
          if (enabled[layer]) {
            auto& popups = frame->get_popups(layer);
            popups.iterate([&](const auto& popup) {
              if (popup.hit_test(mouse_game_pos)) {
                ImGui::BeginTooltip();
                ImGui::Text("%s", popup.text());
                ImGui::EndTooltip();
              }
            });
          }
        }
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
    if (frames[0]) {
      //      for (const auto& frame : frames) {
      const auto& cameras = frames[0]->get_cameras();
      auto it = cameras.find(ui_state.selected_camera);
      if (it != cameras.end()) {
        if (ui_state.ignore_camera_zoom) {
          camera.set_position(it->second.position);
        } else {
          camera.set_view(it->second);
        }
      }
      //      }
    }
  }
  scene.render();

  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
  if (frames[0]) {
    for (size_t i = 0; i < 2; i++) {
      const auto& frame = frames[i];
      const auto& enabled =
          i == 0 ? config.scene->enabled_layers : config.scene->enabled_permanent_layers;
      for (size_t layer = 0; layer < models::Frame::LAYERS_COUNT; layer++) {
        if (enabled[layer]) {
          auto& texts = frame->get_texts(layer);
          texts.iterate([&](const auto& text) {
            // TODO: take origin into account. clean up code.
            glm::vec2 text_position = camera.game_to_screen(text.position, text.origin);
            text_position /= glm::vec2{io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y};

            text_position.x += 1.0f;
            text_position.y += (ui_state.main_menu_height + 1.0f);

            draw_list->AddText(style_manager.draw_font, text.size / camera.get_scale(text.origin), text_position,
                               text.color, text.text.c_str());
          });
        }
      }
    }
  }

  // Cleanup opengl state
  glBindVertexArray(0);
  glUseProgram(0);
}

}  // namespace rewind_viewer::ui
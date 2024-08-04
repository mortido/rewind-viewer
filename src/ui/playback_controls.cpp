#include "ui/playback_controls.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>

namespace rewind_viewer::ui {

void PlaybackControls::render(RewindViewerState& ui_state, const models::Config& config,
                              models::Scene& scene) {
  if (!ui_state.show_playback_controls) {
    ui_state.playback_controls_height = 0.0f;
    return;
  }
  static const auto button_size = ImVec2{0, 0};
  static const float buttons_spacing = 5.0f;
  auto& io = ImGui::GetIO();
  float width = io.DisplaySize.x;
  ui_state.playback_controls_height = 20 + 2 * ImGui::GetStyle().WindowPadding.y;
  ImGui::SetNextWindowPos({0, io.DisplaySize.y - ui_state.playback_controls_height});
  ImGui::SetNextWindowSize({width, ui_state.playback_controls_height});
  static const auto flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings;
  if (ImGui::Begin("Playback control", &ui_state.show_playback_controls, flags)) {
    ImGui::BeginGroup();

    ImGui::PushItemWidth(30);
    if (ImGui::InputInt("FPS", &config.ui->replay_fps, 0)) {
      if (config.ui->replay_fps < 1) {
        config.ui->replay_fps = 1;
      }
      if (config.ui->replay_fps > 240) {
        config.ui->replay_fps = 240;
      }
    }
    ImGui::PopItemWidth();

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_BACKWARD_FAST "##fastprev", button_size)) {
      //    if (ImGui::IsItemActive()) { in case if long press will be needed
      if (ui_state.current_frame_idx > config.ui->fast_skip_speed) {
        ui_state.current_frame_idx -= config.ui->fast_skip_speed;
      } else {
        ui_state.current_frame_idx = 0;
      }
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_BACKWARD "##prev", button_size)) {
      if (ui_state.current_frame_idx > 0) {
        ui_state.current_frame_idx--;
      }
      ui_state.autoplay = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ui_state.autoplay) {
      ui_state.autoplay = !ImGui::Button(ICON_FA_PAUSE "##pause", button_size);
    } else {
      ui_state.autoplay = ImGui::Button(ICON_FA_PLAY "##play", button_size);
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD "##next", button_size)) {
      ui_state.current_frame_idx++;
      ui_state.autoplay = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD_FAST "##fastnext", button_size)) {
      ui_state.current_frame_idx += config.ui->fast_skip_speed;
    }
    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_ROTATE_LEFT "##repeat", button_size)) {
      ui_state.current_frame_idx = 0;
    }

    ImGui::SameLine();
    // Tick is one indexed
    const size_t frames_cnt = scene.frames_count();
    if (frames_cnt > 0ul) {
      ui_state.current_frame_idx = std::min(frames_cnt, ui_state.current_frame_idx);
      ImGui::PushItemWidth(-1);
      int frame = static_cast<int>(ui_state.current_frame_idx) + 1;
      const std::string slider_fmt = "%5d/" + std::to_string(frames_cnt);
      if (ImGui::SliderInt("##empty", &frame, 1, static_cast<int>(frames_cnt), slider_fmt.data(),
                           ImGuiSliderFlags_AlwaysClamp)) {
        ui_state.current_frame_idx = frame - 1;
        ui_state.autoplay = false;
      }
      ImGui::PopItemWidth();
    } else {
      ImGui::TextDisabled("No Frames");
    }

    ImGui::EndGroup();
    ImGui::End();
  }
}

void PlaybackControls::handle_inputs(RewindViewerState& ui_state, const models::Config& config) {
  const auto& io = ImGui::GetIO();
  if (!io.WantTextInput) {
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow)) {
      if (io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl) {
        if (ui_state.current_frame_idx > config.ui->fast_skip_speed) {
          ui_state.current_frame_idx -= config.ui->fast_skip_speed;
        } else {
          ui_state.current_frame_idx = 0;
        }
      } else {
        if (ui_state.current_frame_idx > 0) {
          ui_state.current_frame_idx--;
        }
      }
      ui_state.autoplay = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      if (io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl) {
        ui_state.current_frame_idx += config.ui->fast_skip_speed;
      } else {
        ui_state.current_frame_idx++;
      }
      ui_state.autoplay = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
      ui_state.autoplay = !ui_state.autoplay;
    }

    static constexpr int fps_values[] = {1, 2, 5, 10, 20, 30, 45, 60, 80, 100, 120, 150, 200, 240};
    static constexpr int num_fps_values = sizeof(fps_values) / sizeof(fps_values[0]);
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
      int current_index = std::distance(
          fps_values, std::find(fps_values, fps_values + num_fps_values, config.ui->replay_fps));
      if (current_index < num_fps_values - 1) {
        config.ui->replay_fps = fps_values[++current_index];
      }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
      int current_index = std::distance(
          fps_values, std::find(fps_values, fps_values + num_fps_values, config.ui->replay_fps));
      if (current_index > 0) {
        config.ui->replay_fps = fps_values[--current_index];
      }
    }
  }
}

}  // namespace rewind_viewer::ui
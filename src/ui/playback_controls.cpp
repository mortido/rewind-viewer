#include "ui/playback_controls.h"

#include <imgui/fontawesome.h>
#include <imgui/imgui.h>

namespace rewind_viewer::ui {

void PlaybackControls::render(RewindViewerState& ui_state, const models::Config& config,
                              models::Scene& scene) {
  handle_inputs(config, scene);
  if (!ui_state.show_playback_controls) {
    ui_state.playback_controls_height = 0.0f;
    return;
  }
  size_t frame_idx = scene.get_current_frame_idx();
  size_t prev_frame_idx = frame_idx;
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
      if (frame_idx > config.ui->fast_skip_speed) {
        frame_idx -= config.ui->fast_skip_speed;
      } else {
        frame_idx = 0;
      }
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_BACKWARD "##prev", button_size)) {
      if (frame_idx > 0) {
        frame_idx--;
      }
      autoplay_ = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (autoplay_) {
      autoplay_ = !ImGui::Button(ICON_FA_PAUSE "##pause", button_size);
    } else {
      autoplay_ = ImGui::Button(ICON_FA_PLAY "##play", button_size);
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD "##next", button_size)) {
      frame_idx++;
      autoplay_ = false;
    }

    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_FORWARD_FAST "##fastnext", button_size)) {
      frame_idx += config.ui->fast_skip_speed;
    }
    ImGui::SameLine(0.0f, buttons_spacing);
    if (ImGui::Button(ICON_FA_ROTATE_LEFT "##repeat", button_size)) {
      frame_idx = 0;
    }

    ImGui::SameLine();
    // Tick is one indexed
    const size_t frames_cnt = scene.frames_count();
    if (frames_cnt > 0ul) {
      frame_idx = std::min(frames_cnt - 1, frame_idx);
      ImGui::PushItemWidth(-1);
      int frame = static_cast<int>(frame_idx) + 1;
      const std::string slider_fmt = "%5d/" + std::to_string(frames_cnt);
      if (ImGui::SliderInt("##empty", &frame, 1, static_cast<int>(frames_cnt), slider_fmt.data(),
                           ImGuiSliderFlags_AlwaysClamp)) {
        frame_idx = frame - 1;
        autoplay_ = false;
      }
      ImGui::PopItemWidth();
    } else {
      ImGui::TextDisabled("No Frames");
    }

    if (prev_frame_idx != frame_idx) {
      scene.set_current_frame_idx(frame_idx);
    }

    ImGui::EndGroup();
    ImGui::End();
  }
}

void PlaybackControls::handle_inputs(const models::Config& config, models::Scene& scene) {
  const auto& io = ImGui::GetIO();
  if (!io.WantTextInput) {
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftArrow)) {
      size_t frame_idx = scene.get_current_frame_idx();
      if (io.KeyCtrl) {
        if (frame_idx > config.ui->fast_skip_speed) {
          frame_idx -= config.ui->fast_skip_speed;
        } else {
          frame_idx = 0;
        }
      } else if (frame_idx > 0) {
        frame_idx--;
      }
      scene.set_current_frame_idx(frame_idx);
      autoplay_ = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      size_t frame_idx = scene.get_current_frame_idx();
      if (io.KeyCtrl) {
        frame_idx += config.ui->fast_skip_speed;
      } else {
        frame_idx++;
      }
      scene.set_current_frame_idx(frame_idx);
      autoplay_ = false;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
      autoplay_ = !autoplay_;
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

  if (autoplay_) {
    double delta_time = ImGui::GetTime() - last_frame_time_;
    double frame_time = 1.0 / static_cast<double>(config.ui->replay_fps);
    if (delta_time >= frame_time) {
      scene.set_current_frame_idx(scene.get_current_frame_idx() + 1ul);
      last_frame_time_ = ImGui::GetTime();
    }
  }
}

}  // namespace rewind_viewer::ui
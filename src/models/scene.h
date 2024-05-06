#pragma once

#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <mutex>
#include <string>
#include <vector>

#include "common/lock.h"
#include "gl/renderer.h"
#include "models/camera.h"
#include "models/config.h"
#include "models/frames_sequence.h"

namespace rewind_viewer::models {

class Scene {
 public:
  Scene(const SceneConfig &config, bool buffered_mode)
      : camera{config.camera.position, config.camera.scale, config.camera.y_axis_up}
      , frames{buffered_mode}
      , config_(config)
      , renderer_{config_.shaders_dir, config_.size, config_.grid_cells} {}

  void render() {
    renderer_.new_frame(camera);
    if (config_.show_background) {
      renderer_.render_canvas(config_.background_color);
    }

    if (config_.show_grid) {
      renderer_.render_grid(config_.grid_color);
    }

    auto [perma_frame, frame] = frames.get_frame(&cur_frame_idx_);

    // Draw currently selected frame
    if (frame) {
      auto perma_frame_contexts = perma_frame->all_primitives();
      auto frame_contexts = frame->all_primitives();
      for (size_t idx = 0; idx < Frame::LAYERS_COUNT; ++idx) {
        if (config_.enabled_permanent_layers[idx]) {
          renderer_.render_primitives((*perma_frame_contexts)[idx]);
        }
        if (config_.enabled_layers[idx]) {
          renderer_.render_primitives((*frame_contexts)[idx]);
        }
      }
    }
  }

  /// Show detailed info in tooltip if mouse hover unit
  /// @note Called from render thread
  void show_popup(const glm::vec2 &mouse) {
    // todo: return just string(s), remove ImGui dependency
    auto [perma_frame, frame] = frames.get_frame(&cur_frame_idx_);

    // Draw currently selected frame
    if (frame) {
      auto popups = frame->get_popups();
      for (size_t idx = 0; idx < popups->size(); ++idx) {
        if (!config_.enabled_layers[idx]) {
          continue;
        }
        for (const auto &popup : (*popups)[idx]) {
          if (popup.hit_test(mouse)) {
            ImGui::BeginTooltip();
            ImGui::Text("%s", popup.text());
            ImGui::EndTooltip();
          }
        }
      }
    }
  }

  std::string get_user_text() {
    // todo: optimize
    auto [perma_frame, frame] = frames.get_frame(&cur_frame_idx_);
    if (frame) {
      return frame->get_user_message();
    }
    return "";
  }

  size_t get_frame_index() {
    return cur_frame_idx_;
  }

  void set_frame_index(size_t idx) {
    cur_frame_idx_ = std::min(idx, frames.size());
  }

  void forward_frames(size_t count) {
    cur_frame_idx_ += count;
  }

  void backward_frames(size_t count) {
    if (count >= cur_frame_idx_) {
      cur_frame_idx_ = 0;
    } else {
      cur_frame_idx_ -= count;
    }
  }

  const std::map<std::string, CameraView> *get_cameras() {
    auto [perma_frame, frame] = frames.get_frame(&cur_frame_idx_);
    // Draw currently selected frame
    if (frame) {
      return &frame->get_cameras();
    }
    return nullptr;
  }

  void set_canvas_config(const glm::vec2 &size, const glm::u16vec2 &grid) {
    CameraView view{
        .position = size * 0.5f,
        .viewport = size * 1.25f,
    };
    camera.set_view(view);
    renderer_.set_canvas(size, grid);
  }

  void reset() {
    // todo: lock or atomic
    cur_frame_idx_ = 0;
    frames.clear();
  }

 public:
  models::Camera camera;
  FramesSequence frames;

 private:
  const SceneConfig &config_;
  gl::Renderer renderer_;
  size_t cur_frame_idx_ = 0;
};

}  // namespace rewind_viewer::models

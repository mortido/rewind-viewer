#include "models/frame.h"

#include <sstream>

namespace rewind_viewer::models {

void Frame::transfer_from(Frame& other) {
  std::lock_guard lock1(mutex_);
  std::lock_guard lock2(other.mutex_);
  for (size_t i = 0; i < LAYERS_COUNT; ++i) {
    primitives_indices_[i].transfer_from(primitives_storage_, other.primitives_indices_[i]);
  }
  primitives_storage_.transfer_from(other.primitives_storage_);
}

void Frame::render(const gl::RenderContext& context, gl::Renderer& renderer,
                   const std::array<bool, LAYERS_COUNT>& enabled_layers, bool force_load) {
  std::lock_guard lock(mutex_);
  if (force_load || primitives_storage_.updated) {
    renderer.load_primitives(context, primitives_storage_);
    primitives_storage_.updated = false;
  }
  for (size_t idx = 0; idx < Frame::LAYERS_COUNT; ++idx) {
    if (enabled_layers[idx]) {
      renderer.render_primitives(context, primitives_indices_[idx]);
    }
  }
}

const std::map<std::string, CameraView>& UIFrame::get_cameras() const {
  std::lock_guard lock(mutex_);
  return camera_views_;
}

const std::string& UIFrame::get_user_message() const {
  std::lock_guard lock(mutex_);
  return user_message_;
}

std::string UIFrame::get_popup_text(glm::vec2 mouse_game_pos,
                                    const std::array<bool, LAYERS_COUNT>& enabled_layers) {
  std::stringstream ss;
  bool first = true;
  for (size_t idx = 0; idx < LAYERS_COUNT; ++idx) {
    if (enabled_layers[idx]) {
      for (const auto& popup : popups_[idx]) {
        if (popup.hit_test(mouse_game_pos)) {
          if (first) {
            first = false;
          } else {
            ss << std::endl;
          }
          ss << popup.text();
        }
      }
    }
  }
  return ss.str();
}

}  // namespace rewind_viewer::models

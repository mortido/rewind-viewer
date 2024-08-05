

#include "models/frame_editor.h"

namespace rewind_viewer::models {

void FrameEditor::reset() {
  if (master_) {
    scene_->clear();
  }
  layer_id_ = DEFAULT_LAYER;
  use_permanent_ = false;
  proto_creation_ = false;
  proto_storage_ = gl::PrimitiveStorage();
  protos_indices_.clear();
};

void FrameEditor::set_map(const glm::vec2& position, const glm::vec2& size,
                          const glm::u16vec2& grid) {
  if (master_) {
    scene_->set_canvas_config(position, size, grid);
  } else {
    LOG_WARN("Only master can update scene map");
  }
}

void FrameEditor::set_layer(size_t l, bool p, CameraOrigin alignment) {
  layer_id_ = std::min(l, static_cast<size_t>(Frame::LAYERS_COUNT) - 1);
  use_permanent_ = p;
  projection_idx_ = static_cast<uint32_t>(alignment);
}

void FrameEditor::set_layer_name(size_t l, std::string name, bool p) {
  if (l < Frame::LAYERS_COUNT) {
    scene_->set_layer_name(l, std::move(name), p);
  }
}

void FrameEditor::add_camera_view(const std::string& name, CameraView view) {
  auto frame = scene_->get_ui_frame();
  std::lock_guard lock(frame->mutex_);
  frame->camera_views_[name] = view;
}

void FrameEditor::add_user_text(const std::string& msg) {
  auto frame = scene_->get_ui_frame();
  std::lock_guard lock(frame->mutex_);
  frame->user_message_ += msg;
  frame->user_message_ += '\n';
}

void FrameEditor::add_box_popup(glm::vec2 center, glm::vec2 size, std::string message) {
  auto frame = scene_->get_ui_frame();
  std::lock_guard lock(frame->mutex_);
  frame->popups_[layer_id_].push_back(Popup::create_rect(center, size, std::move(message)));
}

void FrameEditor::add_round_popup(glm::vec2 center, float radius, std::string message) {
  auto frame = scene_->get_ui_frame();
  std::lock_guard lock(frame->mutex_);
  frame->popups_[layer_id_].push_back(Popup::create_circle(center, radius, std::move(message)));
}

void FrameEditor::finish_frame() {
  scene_->commit_frame();
}

}  // namespace rewind_viewer::models

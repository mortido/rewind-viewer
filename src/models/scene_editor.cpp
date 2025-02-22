

#include "models/scene_editor.h"

namespace rewind_viewer::models {

void SceneEditor::reset() {
  if (master_) {
    scene_->reset();
  }
  layer_id_ = DEFAULT_LAYER;
  use_permanent_ = false;
  proto_creation_ = false;
  proto_storage_ = gl::PrimitiveStorage();
  protos_indices_.clear();
};

void SceneEditor::set_map(const glm::vec2& position, const glm::vec2& size,
                          const glm::u16vec2& grid) {
  if (master_) {
    if (scene_->get_current_frame_idx() == 0) {
      CameraView view{position + size * 0.5f, size * 1.25f};
      scene_->get_camera().set_view(view);
    }
    scene_->get_renderer().update_canvas(position, size, grid);
  } else {
    LOG_WARN("Only master can update scene map");
  }
}

void SceneEditor::set_layer(size_t l, bool p, CameraOrigin origin) {
  layer_id_ = std::min(l, static_cast<size_t>(Frame::LAYERS_COUNT) - 1);
  use_permanent_ = p;
  origin_ = origin;
}

void SceneEditor::set_layer_name(size_t l, std::string name, bool p) {
  if (l < Frame::LAYERS_COUNT) {
    scene_->set_layer_name(l, std::move(name), p);
  }
}

void SceneEditor::start_proto() {
  proto_creation_ = true;
  protos_indices_.emplace_back();
}

void SceneEditor::end_proto() {
  proto_creation_ = false;
}

void SceneEditor::add_proto(size_t idx, glm::vec2 position, float angle, uint32_t color,
                            float scale) {
  auto frame = scene_->get_buffer_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].copy_from(
      frame->primitives_storage_, proto_storage_, protos_indices_[idx],
      static_cast<uint32_t>(origin_), position, angle, color, scale);
}

void SceneEditor::add_camera_view(const std::string& name, CameraView view) {
  auto frame = scene_->get_buffer_frame(false);  // TODO: permanent ui features?
  std::lock_guard lock(frame->mutex_);
  frame->camera_views_[name] = view;
}

void SceneEditor::add_user_text(const std::string& msg) {
  auto frame = scene_->get_buffer_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->user_message_ += msg;
  frame->user_message_ += '\n';
}

void SceneEditor::add_box_popup(glm::vec2 center, glm::vec2 size, std::string message) {
  auto frame = scene_->get_buffer_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->popups_[layer_id_].emplace(Popup::create_rect(center, size, std::move(message)));
}

void SceneEditor::add_round_popup(glm::vec2 center, float radius, std::string message) {
  auto frame = scene_->get_buffer_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->popups_[layer_id_].emplace(Popup::create_circle(center, radius, std::move(message)));
}

void SceneEditor::add_text(glm::vec2 position, float size, uint32_t color, std::string message) {
  auto frame = scene_->get_buffer_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->texts_[layer_id_].emplace(Text{std::move(message), position, size, color, origin_});
}

void SceneEditor::finish_frame() {
  scene_->commit_frame();
}

}  // namespace rewind_viewer::models

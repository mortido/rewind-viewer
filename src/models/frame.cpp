#include "models/frame.h"

namespace rewind_viewer::models {

void Frame::transfer_from(Frame& other) {
  std::lock_guard<Spinlock> lock1(mutex_);
  std::lock_guard<Spinlock> lock2(other.mutex_);
  for (size_t i = 0; i < contexts_.size(); ++i) {
    contexts_[i].update_from(other.contexts_[i]);
    other.contexts_[i].clear();
  }
}

ScopeLockedRefWrapper<const Frame::ContextCollectionT, Spinlock> Frame::all_contexts() const {
  return {contexts_, mutex_};
}

ScopeLockedRefWrapper<render::RenderContext, Spinlock> Frame::get_context(size_t layer) {
  layer = std::min(layer, LAYERS_COUNT - 1ul);
  return {contexts_[layer], mutex_};
}

void UIFrame::add_camera_view(const std::string& name, CameraView view) {
  camera_views_[name] = view;
}

const std::map<std::string, CameraView>& UIFrame::get_cameras() const {
  return camera_views_;
}

void UIFrame::add_user_text(const std::string& msg) {
  std::lock_guard<Spinlock> lock(mutex_);
  user_message_ += msg;
  user_message_ += '\n';
}

const std::string& UIFrame::get_user_message() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return user_message_;
}

void UIFrame::add_box_popup(size_t layer, glm::vec2 center, glm::vec2 size, std::string message) {
  std::lock_guard<Spinlock> lock(mutex_);
  layer = std::min(layer, LAYERS_COUNT - 1ul);
  popups_[layer].push_back(Popup::create_rect(center, size, std::move(message)));
}

void UIFrame::add_round_popup(size_t layer, glm::vec2 center, float radius, std::string message) {
  std::lock_guard<Spinlock> lock(mutex_);
  layer = std::min(layer, LAYERS_COUNT - 1ul);
  popups_[layer].push_back(Popup::create_circle(center, radius, std::move(message)));
}

ScopeLockedRefWrapper<const UIFrame::PopupCollectionT, Spinlock> UIFrame::get_popups() {
  return {popups_, mutex_};
}

}  // namespace rewind_viewer::models

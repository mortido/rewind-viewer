

#include "models/frame_editor.h"

namespace rewind_viewer::models {

void FrameEditor::reset() {
  if (master_) {
    scene_->clear();
  }
  layer_id_ = DEFAULT_LAYER;
  use_permanent_ = false;
};

void FrameEditor::set_map(const glm::vec2& position, const glm::vec2& size,
                          const glm::u16vec2& grid) {
  if (master_) {
    scene_->set_canvas_config(position, size, grid);
  } else {
    LOG_WARN("Only master can update scene map");
  }
}

void FrameEditor::set_layer(size_t l, bool p) {
  layer_id_ = std::min(l, Frame::LAYERS_COUNT - 1);
  use_permanent_ = p;
}

void FrameEditor::add_arc(glm::vec2 center, float r, float start_angle, float end_angle,
                          uint32_t color, bool fill) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_arc(frame->primitives_storage_, center, r, start_angle,
                                                end_angle, color, fill);
}
void FrameEditor::add_circle(glm::vec2 center, float r, uint32_t color, bool fill) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_circle(frame->primitives_storage_, center, r, color,
                                                   fill);
}
void FrameEditor::add_segment(glm::vec2 center, float r, float start_angle, float end_angle,
                              uint32_t color, bool fill) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_segment(frame->primitives_storage_, center, r,
                                                    start_angle, end_angle, color, fill);
}
void FrameEditor::add_polyline(const std::vector<glm::vec2>& points, uint32_t color) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_polyline(frame->primitives_storage_, points, color);
}
void FrameEditor::add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, uint32_t color,
                               bool fill) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_triangle(frame->primitives_storage_, p1, p2, p3, color,
                                                     fill);
}
void FrameEditor::add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right, uint32_t color,
                                bool fill) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_rectangle(frame->primitives_storage_, top_left,
                                                      bottom_right, color, fill);
}
void FrameEditor::add_stencil_arc(glm::vec2 center, float r, float start_angle, float end_angle) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_stencil_arc(frame->primitives_storage_, center, r,
                                                        start_angle, end_angle);
}
void FrameEditor::add_stencil_circle(glm::vec2 center, float r) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_stencil_circle(frame->primitives_storage_, center, r);
}
void FrameEditor::add_stencil_segment(glm::vec2 center, float r, float start_angle,
                                      float end_angle) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_stencil_segment(frame->primitives_storage_, center, r,
                                                            start_angle, end_angle);
}
void FrameEditor::add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_stencil_triangle(frame->primitives_storage_, p1, p2,
                                                             p3);
}
void FrameEditor::add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right) {
  auto frame = scene_->get_draw_frame(use_permanent_);
  std::lock_guard lock(frame->mutex_);
  frame->primitives_indices_[layer_id_].add_stencil_rectangle(frame->primitives_storage_, top_left,
                                                              bottom_right);
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

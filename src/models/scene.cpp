#include "models/scene.h"

#include <utility>

namespace rewind_viewer::models {

Scene::Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode)
    : camera{config->camera.position, config->camera.scale, config->camera.y_axis_up}
    , config_{std::move(config)}
    , frame_context_{gl_resources_}
    , permanent_frame_context_{gl_resources_}
    , renderer_{gl_resources_, config_->shaders_dir, config_->canvas_position, config_->canvas_size,
                config_->grid_cells}
    , buffered_mode_{buffered_mode}
    , buffer_frame_{std::make_shared<UIFrame>()}
    , buffer_permanent_frame_{std::make_shared<Frame>()}
    , permanent_frame_{std::make_shared<Frame>()} {}

void Scene::render(size_t frame_idx) {
  renderer_.new_frame();
  renderer_.set_projections(camera.get_projection_matrices());
  if (config_->show_background) {
    renderer_.render_canvas(config_->background_color);
  }
  if (config_->show_grid) {
    renderer_.render_grid(config_->grid_color);
  }

  auto frame = get_frame(&frame_idx);
  if (frame) {
    permanent_frame_->render(permanent_frame_context_, renderer_, config_->enabled_permanent_layers);
    frame->render(frame_context_, renderer_, config_->enabled_layers,
                  prev_rendered_frame_ != frame_idx);
    prev_rendered_frame_ = frame_idx;
  }
}

void Scene::set_canvas_config(const glm::vec2& position, const glm::vec2& size,
                              const glm::u16vec2& grid) {
  // todo: only on first frame...
  CameraView view{position + size * 0.5f, size * 1.25f};
  camera.set_view(view);

  std::lock_guard<Spinlock> lock(mutex_);

  //  frames_.clear();
  //  buffer_frame_ = std::make_shared<UIFrame>();
  //  permanent_frame_ = std::make_shared<Frame>();
  //  buffer_permanent_frame_ = std::make_shared<Frame>();
  renderer_.update_canvas(position, size, grid);
}

size_t Scene::frames_count() const {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    return frames_.size();
  } else {
    return frames_.size() + 1;
  }
}

std::shared_ptr<UIFrame> Scene::get_frame(size_t* idx) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    if (frames_.empty()) {
      return nullptr;
    }
    *idx = std::min(*idx, frames_.size() - 1ul);
  } else {
    *idx = std::min(*idx, frames_.size());
    if (*idx == frames_.size()) {
      return buffer_frame_;
    }
  }
  return frames_[*idx];
}

void Scene::set_buffered_mode(bool mode) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_ == mode) {
    return;
  }
  buffered_mode_ = mode;
  if (!buffered_mode_) {
    permanent_frame_->transfer_from(*buffer_permanent_frame_);
  }
}

void Scene::clear() {
  std::lock_guard<Spinlock> lock(mutex_);
  frames_.clear();
  buffer_frame_ = std::make_shared<UIFrame>();
  permanent_frame_ = std::make_shared<Frame>();
  buffer_permanent_frame_ = std::make_shared<Frame>();
  permanent_layers_names_.fill("");
  layers_names_.fill("");
}

std::shared_ptr<UIFrame> Scene::get_ui_frame() {
  std::lock_guard<Spinlock> lock(mutex_);
  return buffer_frame_;
}

std::shared_ptr<Frame> Scene::get_draw_frame(bool permanent) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    return permanent ? buffer_permanent_frame_ : buffer_frame_;
  } else {
    return permanent ? permanent_frame_ : buffer_frame_;
  }
}

void Scene::set_layer_name(size_t id, std::string name, bool permanent) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (permanent) {
    permanent_layers_names_.at(id) = std::move(name);
  } else {
    layers_names_.at(id) = std::move(name);
  }
}

std::string Scene::get_layer_name(size_t id, bool permanent) const {
  std::lock_guard<Spinlock> lock(mutex_);
  if (permanent) {
    return permanent_layers_names_.at(id);
  } else {
    return layers_names_.at(id);
  }
}

void Scene::commit_frame() {
  std::lock_guard<Spinlock> lock(mutex_);
  frames_.emplace_back(std::move(buffer_frame_));
  buffer_frame_ = std::make_shared<UIFrame>();
  permanent_frame_->transfer_from(*buffer_permanent_frame_);
}

}  // namespace rewind_viewer::models

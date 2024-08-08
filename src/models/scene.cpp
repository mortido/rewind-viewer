#include "models/scene.h"

#include <utility>

namespace rewind_viewer::models {

Scene::Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode)
    : camera_{config->camera.position, config->camera.scale, config->camera.y_axis_up}
    , config_{std::move(config)}
    , frame_context_{gl_resources_}
    , permanent_frame_context_{gl_resources_}
    , renderer_{gl_resources_, config_->shaders_dir, config_->canvas_position, config_->canvas_size,
                config_->grid_cells}
    , buffered_mode_{buffered_mode} {
  reset();
}

void Scene::render() {
  camera_.update_projections();
  renderer_.new_frame(camera_.get_projection_matrices());
  if (config_->show_background) {
    renderer_.render_canvas(config_->background_color);
  }
  if (config_->show_grid) {
    renderer_.render_grid(config_->grid_color);
  }

  if (frames_count_ > 0) {
    permanent_frame_->render(permanent_frame_context_, renderer_,
                             config_->enabled_permanent_layers);
    frames_[current_frame_]->render(frame_context_, renderer_, config_->enabled_layers,
                  prev_rendered_frame_ != current_frame_);
    prev_rendered_frame_ = current_frame_;
  }
}

size_t Scene::get_current_frame_idx() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return current_frame_;
}

void Scene::set_current_frame_idx(size_t frame_idx) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (frames_count_ > 0) {
    current_frame_ = std::min(frame_idx, frames_count_ - 1ul);
  } else {
    current_frame_ = 0ul;
  }
}

size_t Scene::frames_count() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return frames_count_;
}

std::shared_ptr<Frame> Scene::get_current_frame(bool permanent) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (frames_count_ > 0) {
    return permanent ? permanent_frame_ : frames_[current_frame_];
  }
  return nullptr;
}

bool Scene::get_buffered_mode() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return buffered_mode_;
}

void Scene::set_buffered_mode(bool mode) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_ == mode) {
    return;
  }
  buffered_mode_ = mode;
  if (!buffered_mode_) {
    permanent_frame_->transfer_from(*permanent_buffer_frame_);
  }
  frames_count_ = frames_.size() - buffered_mode_;
}

void Scene::reset() {
  std::lock_guard<Spinlock> lock(mutex_);
  frames_.clear();
  frames_.emplace_back(std::make_shared<Frame>());
  frames_count_ = frames_.size() - buffered_mode_;
  current_frame_ = 0;
  permanent_frame_ = std::make_shared<Frame>();
  permanent_buffer_frame_ = std::make_shared<Frame>();
  permanent_layers_names_.fill("");
  layers_names_.fill("");
}

std::shared_ptr<Frame> Scene::get_buffer_frame(bool permanent) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    return permanent ? permanent_buffer_frame_ : frames_.back();
  } else {
    return permanent ? permanent_frame_ : frames_.back();
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
  frames_.emplace_back(std::make_shared<Frame>());
  frames_count_++;
  permanent_frame_->transfer_from(*permanent_buffer_frame_);
}

}  // namespace rewind_viewer::models

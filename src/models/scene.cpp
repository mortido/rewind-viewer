#include "models/scene.h"

#include <utility>

namespace rewind_viewer::models {

Scene::Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode)
    : camera{config->camera.position, config->camera.scale, config->camera.y_axis_up}
    , frames{buffered_mode}
    , config_{std::move(config)}
    , renderer_{config_->shaders_dir, config_->canvas_position, config_->canvas_size,
                config_->grid_cells} {}

void Scene::render(size_t frame_idx) {
  renderer_.new_frame(camera);
  if (config_->show_background) {
    renderer_.render_canvas(config_->background_color);
  }

  if (config_->show_grid) {
    renderer_.render_grid(config_->grid_color);
  }

  auto [perma_frame, frame] = frames.get_frame(&frame_idx);

  // Draw currently selected frame
  if (frame) {
    auto perma_frame_contexts = perma_frame->all_primitives();
    auto frame_contexts = frame->all_primitives();
    for (size_t idx = 0; idx < Frame::LAYERS_COUNT; ++idx) {
      if (config_->enabled_permanent_layers[idx]) {
        renderer_.render_primitives((*perma_frame_contexts)[idx]);
      }
      if (config_->enabled_layers[idx]) {
        renderer_.render_primitives((*frame_contexts)[idx]);
      }
    }
  }
}

void Scene::set_canvas_config(const glm::vec2& position, const glm::vec2& size,
                              const glm::u16vec2& grid) {
  CameraView view{
      .position = position + size * 0.5f,
      .viewport = size * 1.25f,
  };
  camera.set_view(view);
  renderer_.update_canvas(position, size, grid);
}

}  // namespace rewind_viewer::models

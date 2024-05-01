//
// Created by valdemar on 29.11.18.
//

#pragma once

#include <memory>

#include "RenderContext.h"
#include "ResourceManager.h"
#include "common/lock.h"
#include "glm/glm.hpp"
#include "models/camera.h"
#include "shaders_collection.h"

namespace rewind_viewer::render {

class Renderer {
 public:
  Renderer(const std::string &shaders_dir, glm::u32vec2 area_size, glm::u16vec2 grid_cells);
  ~Renderer();

  void update_frustum(const models::Camera &cam);
  void render_background(glm::vec3 color);
  void render_grid(glm::vec3 color);
  void render_primitives(const RenderContext &ctx);
  void set_canvas_config(const glm::vec2 &size, const glm::u16vec2 &grid);

 private:
  std::unique_ptr<ResourceManager> resources_;
  std::unique_ptr<ShadersCollection> shaders_;
  RenderContext::context_vao_t ctx_render_params_;

  struct render_attrs_t;
  std::unique_ptr<render_attrs_t> attr_;

  glm::vec2 area_size_;
  glm::u16vec2 grid_cells_;
  Spinlock grid_mutex_;
};

}  // namespace rewind_viewer::models

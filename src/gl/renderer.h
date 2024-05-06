#pragma once

#include <glm/glm.hpp>

#include "common/lock.h"
#include "gl/primitives_collection.h"
#include "gl/render_context.h"
#include "gl/resource_manager.h"
#include "gl/shader.h"
#include "gl/shaders_collection.h"
#include "models/camera.h"

namespace rewind_viewer::gl {

class Renderer {
 public:
  Renderer(const std::string &shaders_dir, glm::u32vec2 area_size, glm::u16vec2 grid_cells);

  void new_frame(const models::Camera &cam);
  void render_canvas(glm::vec3 color);
  void render_grid(glm::vec3 color);
  void render_primitives(const PrimitivesCollection &primitives);

  // TODO: support position for canvas
  void set_canvas(const glm::vec2 &size, const glm::u16vec2 &grid);

 private:
  ResourceManager resources_;
  RenderContext context_;
  ShadersCollection shaders_;
  Spinlock canvas_mutex_;

  GLuint grid_vao = 0;
  GLuint grid_vbo = 0;
  std::vector<float> grid;
  GLsizei grid_vertex_count = 0;
  glm::mat4 grid_model{};

  GLuint canvas_vao = 0;
  GLuint canvas_vbo = 0;
  glm::mat4 canvas_model{};

  GLuint uniform_buf{};

  GLint stencil_ref = 1;




  void init_canvas();
  void init_grid();
  void update_canvas(const glm::vec2 &size);
  void update_grid(const glm::vec2 &canvas_size, const glm::u16vec2 &grid);
};

}  // namespace rewind_viewer::gl

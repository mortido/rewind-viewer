#include "gl/renderer.h"

#include <mutex>

#include "common/logger.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "utils.h"

namespace {

void load_indices(const std::vector<GLuint> &indices) {
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)),
               indices.data(), GL_DYNAMIC_DRAW);
}

}  // anonymous namespace

namespace rewind_viewer::gl {

Renderer::Renderer(const std::string &shaders_dir, const glm::vec2 &canvas_position,
                   const glm::vec2 &canvas_size, const glm::u16vec2 &grid_cells)
    : context_{resources_}, shaders_{shaders_dir} {
  LOG_INFO("Initialize canvas");
  init_canvas();
  update_canvas(canvas_position, canvas_size, grid_cells);

  // Uniform buffer
  LOG_INFO("Create Uniform buffer");
  uniform_buf = resources_.gen_buffer();
  glBindBuffer(GL_UNIFORM_BUFFER, uniform_buf);
  glBufferData(GL_UNIFORM_BUFFER, 64, nullptr, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniform_buf);

  LOG_INFO("Bind Uniform buffer to shaders");
  shaders_.uniform_color.bind_uniform_block("MatrixBlock", 0);
  shaders_.uniform_color_model.bind_uniform_block("MatrixBlock", 0);
  shaders_.uniform_color_circle.bind_uniform_block("MatrixBlock", 0);
  shaders_.color_pass.bind_uniform_block("MatrixBlock", 0);
  shaders_.color_circle.bind_uniform_block("MatrixBlock", 0);
}

void Renderer::init_canvas() {
  LOG_INFO("Create rectangle for future rendering");
  canvas_vao = resources_.gen_vertex_array();
  canvas_vbo = resources_.gen_buffer();
  //@formatter:off
  const float points[] = {
      0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
  };
  //@formatter:on
  glBindVertexArray(canvas_vao);
  glBindBuffer(GL_ARRAY_BUFFER, canvas_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  grid_vao = resources_.gen_vertex_array();
  grid_vbo = resources_.gen_buffer();

  glBindVertexArray(grid_vao);
  glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}

void Renderer::update_canvas([[maybe_unused]] const glm::vec2 &position, const glm::vec2 &size,
                             const glm::u16vec2 &cells) {
  std::lock_guard<Spinlock> lock(canvas_mutex_);

  // TODO: support position for canvas model
  canvas_model = glm::scale(glm::mat4(1.0f), {size, 1.0f});
  //  canvas_model = glm::translate(canvas_model, {position,1.0});

  grid.clear();
  const float step_x = 1.0f / static_cast<float>(cells.x);
  for (size_t i = 0; i <= cells.x; ++i) {
    const float shift = step_x * i;

    grid.push_back(shift);
    grid.push_back(0.0);
    grid.push_back(0.1);

    grid.push_back(shift);
    grid.push_back(1.0);
    grid.push_back(0.1);
  }

  const float step_y = 1.0f / static_cast<float>(cells.y);
  for (size_t i = 0; i <= cells.y; ++i) {
    const float shift = step_y * i;

    grid.push_back(0.0);
    grid.push_back(shift);
    grid.push_back(0.1);

    grid.push_back(1.0);
    grid.push_back(shift);
    grid.push_back(0.1);
  }

  grid_vertex_count = static_cast<GLsizei>(grid.size() / 3);
}

void Renderer::new_frame(const models::Camera &cam) {
  // Update projection matrix
  glBindBuffer(GL_UNIFORM_BUFFER, uniform_buf);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(cam.get_projection_matrix()),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);

  glStencilMask(0xFF);
  glClear(GL_STENCIL_BUFFER_BIT);
  glStencilMask(0x00);
  stencil_ref = 1;
}

void Renderer::render_canvas(glm::vec3 color) {
  std::lock_guard<Spinlock> lock(canvas_mutex_);
  shaders_.uniform_color_model.use();
  shaders_.uniform_color_model.set_mat4("model", canvas_model);
  shaders_.uniform_color_model.set_vec4("color", glm::vec4{color, 1.0f});
  glBindVertexArray(canvas_vao);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}

void Renderer::render_grid(glm::vec3 color) {
  std::lock_guard<Spinlock> lock(canvas_mutex_);
  if (!grid.empty()) {
    glBindBuffer(GL_ARRAY_BUFFER, grid_vbo);
    glBufferData(GL_ARRAY_BUFFER, grid.size() * sizeof(float), grid.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    grid.clear();
  }

  shaders_.uniform_color_model.use();
  shaders_.uniform_color_model.set_mat4("model", canvas_model);
  shaders_.uniform_color_model.set_vec4("color", glm::vec4{color, 1.0f});
  glBindVertexArray(grid_vao);
  glDrawArrays(GL_LINES, 0, grid_vertex_count);
  glBindVertexArray(0);
}

void Renderer::load_primitives(const PrimitivesStorage &storage) {
  glCheckError();
  glBindBuffer(GL_ARRAY_BUFFER, context_.color_vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(storage.color_vertexes.size() * sizeof(ColorVertex)),
               storage.color_vertexes.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, context_.color_circle_vbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(storage.color_circles.size() * sizeof(ColorCircle)),
               storage.color_circles.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, context_.vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(storage.vertexes.size() * sizeof(Vertex)),
               storage.vertexes.data(), GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, context_.circle_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(storage.circles.size() * sizeof(Circle)),
               storage.circles.data(), GL_DYNAMIC_DRAW);
}

void Renderer::render_primitives(const PrimitivesCollection &primitives) {
//  glCheckError();
  // glLineWidth(2);
  // glEnable(GL_LINE_SMOOTH);

  // Load data
  //  glBindBuffer(GL_ARRAY_BUFFER, context_.color_vertex_vbo);
  //  glBufferData(GL_ARRAY_BUFFER,
  //               static_cast<GLsizeiptr>(primitives.color_vertexes.size() * sizeof(ColorVertex)),
  //               primitives.color_vertexes.data(), GL_DYNAMIC_DRAW);
  //
  //  glBindBuffer(GL_ARRAY_BUFFER, context_.color_circle_vbo);
  //  glBufferData(GL_ARRAY_BUFFER,
  //               static_cast<GLsizeiptr>(primitives.color_circles.size() * sizeof(ColorCircle)),
  //               primitives.color_circles.data(), GL_DYNAMIC_DRAW);
  //
  //  glBindBuffer(GL_ARRAY_BUFFER, context_.vertex_vbo);
  //  glBufferData(GL_ARRAY_BUFFER,
  //               static_cast<GLsizeiptr>(primitives.vertexes.size() * sizeof(Vertex)),
  //               primitives.vertexes.data(), GL_DYNAMIC_DRAW);
  //
  //  glBindBuffer(GL_ARRAY_BUFFER, context_.circle_vbo);
  //  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(primitives.circles.size() *
  //  sizeof(Circle)),
  //               primitives.circles.data(), GL_DYNAMIC_DRAW);

  // Stencil drawings
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS, stencil_ref, 0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  glStencilMask(0xFF);

  shaders_.uniform_color.use();
  glBindVertexArray(context_.vertex_vao);
  load_indices(primitives.stencil_triangles);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(primitives.stencil_triangles.size()),
                 GL_UNSIGNED_INT, nullptr);

  shaders_.uniform_color_circle.use();
  shaders_.uniform_color_circle.set_int("is_segment", 0);
  glBindVertexArray(context_.circle_vao);
  load_indices(primitives.stencil_circles);
  glDrawElements(GL_POINTS, static_cast<GLsizei>(primitives.stencil_circles.size()),
                 GL_UNSIGNED_INT, nullptr);

  shaders_.uniform_color_circle.set_int("is_segment", 1);
  glBindVertexArray(context_.circle_vao);
  load_indices(primitives.stencil_segments);
  glDrawElements(GL_POINTS, static_cast<GLsizei>(primitives.stencil_segments.size()),
                 GL_UNSIGNED_INT, nullptr);

  // Normal drawings
  glStencilFunc(GL_NOTEQUAL, stencil_ref, 0xFF);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glStencilMask(0x00);

  // Simple pass shader - triangles and lines
  shaders_.color_pass.use();
  glBindVertexArray(context_.color_vertex_vao);
  load_indices(primitives.triangles);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(primitives.triangles.size()), GL_UNSIGNED_INT,
                 nullptr);

  load_indices(primitives.lines);
  glDrawElements(GL_LINES, static_cast<GLsizei>(primitives.lines.size()), GL_UNSIGNED_INT, nullptr);

  // Circles shader
  shaders_.color_circle.use();
  glBindVertexArray(context_.color_circle_vao);
  shaders_.color_circle.set_int("is_segment", 0);
  shaders_.color_circle.set_uint("line_width", 1);
  load_indices(primitives.thin_circles);
  glDrawElements(GL_POINTS, static_cast<GLsizei>(primitives.thin_circles.size()), GL_UNSIGNED_INT,
                 nullptr);

  shaders_.color_circle.set_uint("line_width", 0);
  load_indices(primitives.filled_circles);
  glDrawElements(GL_POINTS, static_cast<GLsizei>(primitives.filled_circles.size()), GL_UNSIGNED_INT,
                 nullptr);

  shaders_.color_circle.set_int("is_segment", 1);
  load_indices(primitives.filled_segments);
  glDrawElements(GL_POINTS, static_cast<GLsizei>(primitives.filled_segments.size()),
                 GL_UNSIGNED_INT, nullptr);

  // glLineWidth(1);
  // glDisable(GL_LINE_SMOOTH);
  glDisable(GL_STENCIL_TEST);
  stencil_ref++;
  glBindVertexArray(0);
  glCheckError();
}

}  // namespace rewind_viewer::gl

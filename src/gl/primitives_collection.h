#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "gl/models.h"
#include "gl/opengl.h"

namespace rewind_viewer::gl {

struct PrimitiveStorage {
  std::vector<ColorVertex> color_vertexes;
  std::vector<ColorCircle> color_circles;
  std::vector<Vertex> vertexes;
  std::vector<Circle> circles;
  bool updated = false;
  void transfer_from(PrimitiveStorage&other);
};

struct PrimitiveIndices {
  std::vector<GLuint> stencil_circles;
  std::vector<GLuint> stencil_segments;
  std::vector<GLuint> stencil_triangles;
  std::vector<GLuint> filled_circles;
  std::vector<GLuint> filled_segments;
  std::vector<GLuint> thin_circles;
  std::vector<GLuint> triangles;
  std::vector<GLuint> lines;

  void add_arc(PrimitiveStorage& storage, uint32_t projection_idx, glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_circle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 center, float r, uint32_t color, bool fill);
  void add_segment(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_polyline(PrimitiveStorage& storage,uint32_t projection_idx, const std::vector<glm::vec2> &points, uint32_t color);
  void add_triangle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, uint32_t color, bool fill);
  void add_rectangle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 top_left, glm::vec2 bottom_right, uint32_t color, bool fill);
  void add_stencil_arc(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_circle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 center, float r);
  void add_stencil_segment(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_triangle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
  void add_stencil_rectangle(PrimitiveStorage& storage,uint32_t projection_idx, glm::vec2 top_left, glm::vec2 bottom_right);
  void transfer_from(const PrimitiveStorage& storage, PrimitiveIndices&other);
  void copy_from(PrimitiveStorage& storage, const PrimitiveStorage& other_storage, const PrimitiveIndices&other,uint32_t projection_idx, glm::vec2 position, float angle, uint32_t color, float scale);
};

}  // namespace rewind_viewer::gl

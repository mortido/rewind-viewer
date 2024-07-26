#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "gl/models.h"
#include "gl/opengl.h"

namespace rewind_viewer::gl {

struct PrimitivesStorage{
  std::vector<ColorVertex> color_vertexes;
  std::vector<ColorCircle> color_circles;
  std::vector<Vertex> vertexes;
  std::vector<Circle> circles;
  void transfer_from(PrimitivesStorage &other);
};

struct PrimitivesCollection {
  std::shared_ptr<PrimitivesStorage> buffer;

  std::vector<GLuint> stencil_circles;
  std::vector<GLuint> stencil_segments;
  std::vector<GLuint> stencil_triangles;
  std::vector<GLuint> filled_circles;
  std::vector<GLuint> filled_segments;
  std::vector<GLuint> thin_circles;
  std::vector<GLuint> triangles;
  std::vector<GLuint> lines;

  void add_arc(glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_circle(glm::vec2 center, float r, uint32_t color, bool fill);
  void add_segment(glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_polyline(const std::vector<glm::vec2> &points, uint32_t color);
  void add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, uint32_t color, bool fill);
  void add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right, uint32_t color, bool fill);
  void add_stencil_arc(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_circle(glm::vec2 center, float r);
  void add_stencil_segment(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
  void add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right);

  void transfer_from(PrimitivesCollection &other);
};

}  // namespace rewind_viewer::gl

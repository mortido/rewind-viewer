#pragma once

#include <array>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "gl/models.h"
#include "gl/opengl.h"

namespace rewind_viewer::gl {

class PrimitivesCollection {
 public:
  using TriangleColors = std::array<glm::vec4, 3>;
  using RectangleColors = std::array<glm::vec4, 4>;

  void add_arc(glm::vec2 center, float r, float start_angle, float end_angle, glm::vec4 color,
               bool fill);
  void add_circle(glm::vec2 center, float r, glm::vec4 color, bool fill);
  void add_segment(glm::vec2 center, float r, float start_angle, float end_angle, glm::vec4 color,
                   bool fill);
  void add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color, bool fill);
  void add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right, glm::vec4 color, bool fill);
  void add_polyline(const std::vector<glm::vec2> &points, glm::vec4 color);

  void add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const TriangleColors &colors,
                    bool fill);
  void add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right, const RectangleColors &colors,
                     bool fill);

  void add_stencil_arc(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_circle(glm::vec2 center, float r);
  void add_stencil_segment(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
  void add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right);

  void update_from(const PrimitivesCollection &other);
  void clear();

  std::vector<ColorVertex> color_vertexes;
  std::vector<ColorCircle> color_circles;
  std::vector<Vertex> vertexes;
  std::vector<Circle> circles;

  std::vector<GLuint> stencil_circle_indices;
  std::vector<GLuint> stencil_segment_indices;
  std::vector<GLuint> stencil_triangle_indices;
  std::vector<GLuint> filled_circle_indices;
  std::vector<GLuint> filled_segment_indices;
  std::vector<GLuint> thin_circle_indices;
  std::vector<GLuint> triangle_indices;
  std::vector<GLuint> line_indices;
};

}  // namespace rewind_viewer::gl

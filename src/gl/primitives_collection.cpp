#include "gl/primitives_collection.h"

#include <glm/gtc/constants.hpp>
#include <stdexcept>

namespace {

void add_elements(size_t shift, std::vector<GLuint> &to, const std::vector<GLuint> &from) {
  to.reserve(to.size() + from.size());
  for (GLuint i : from) {
    to.push_back(shift + i);
  }
}

inline float normalize_angle(float angle) {
  angle = glm::mod(angle, 2.0f * glm::pi<float>());
  if (angle > glm::pi<float>()) {
    angle -= 2.0f * glm::pi<float>();
  }

  return angle;
}

}  // anonymous namespace

namespace rewind_viewer::gl {

void PrimitivesCollection::add_arc(glm::vec2 center, float r, float start_angle, float end_angle,
                                   glm::vec4 color, bool fill) {
  GLuint idx = color_circles.size();
  color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  if (fill) {
    filled_circle_indices.push_back(idx);
  } else {
    thin_circle_indices.push_back(idx);
  }
}

void PrimitivesCollection::add_circle(glm::vec2 center, float r, glm::vec4 color, bool fill) {
  GLuint idx = color_circles.size();
  color_circles.push_back({color, center, r, 0.0, 0.0});
  if (fill) {
    filled_circle_indices.push_back(idx);
  } else {
    thin_circle_indices.push_back(idx);
  }
}

void PrimitivesCollection::add_segment(glm::vec2 center, float r, float start_angle,
                                       float end_angle, glm::vec4 color, bool fill) {
  GLuint idx = color_circles.size();
  color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  if (fill) {
    filled_segment_indices.push_back(idx);
  } else {
    thin_circle_indices.push_back(idx);  // same logic as circle for shader

    color_vertexes.push_back({color, center});
    color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(start_angle), glm::sin(start_angle))});
    color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(end_angle), glm::sin(end_angle))});

    // Add line between two points in sequence
    GLuint idx = color_vertexes.size() - 1;
    line_indices.push_back(idx - 2);
    line_indices.push_back(idx - 1);
    line_indices.push_back(idx - 2);
    line_indices.push_back(idx);
  }
}

void PrimitivesCollection::add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec4 color,
                                        bool fill) {
  add_triangle(p1, p2, p3, {color, color, color}, fill);
}

void PrimitivesCollection::add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right,
                                         glm::vec4 color, bool fill) {
  RectangleColors colors;
  colors.fill(color);
  add_rectangle(top_left, bottom_right, colors, fill);
}

void PrimitivesCollection::add_polyline(const std::vector<glm::vec2> &points, glm::vec4 color) {
  if (points.size() < 2) {
    throw std::invalid_argument("Cannot create polyline from one point");
  }

  color_vertexes.push_back({color, points[0]});
  for (size_t i = 1; i < points.size(); ++i) {
    color_vertexes.push_back({color, points[i]});

    // Add line between two points in sequence
    GLuint idx = color_vertexes.size() - 1;
    line_indices.push_back(idx - 1);
    line_indices.push_back(idx);
  }
}

void PrimitivesCollection::add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3,
                                        const TriangleColors &colors, bool fill) {
  if (fill) {
    GLuint idx = color_vertexes.size();
    color_vertexes.push_back({colors[0], p1});
    color_vertexes.push_back({colors[1], p2});
    color_vertexes.push_back({colors[2], p3});
    triangle_indices.push_back(idx);
    triangle_indices.push_back(idx + 1);
    triangle_indices.push_back(idx + 2);
  } else {
    add_polyline({p1, p2, p3, p1}, colors[0]);
  }
}

void PrimitivesCollection::add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right,
                                         const RectangleColors &colors, bool fill) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};

  if (fill) {
    GLuint idx = color_vertexes.size();
    color_vertexes.push_back({colors[0], top_left});
    color_vertexes.push_back({colors[1], bottom_left});
    color_vertexes.push_back({colors[2], top_right});
    color_vertexes.push_back({colors[3], bottom_right});

    for (uint8_t t : {0, 2, 1, 2, 3, 1}) {
      triangle_indices.push_back(idx + t);
    }
  } else {
    add_polyline({top_left, top_right, bottom_right, bottom_left, top_left}, colors[0]);
  }
}

void PrimitivesCollection::add_stencil_arc(glm::vec2 center, float r, float start_angle,
                                           float end_angle) {
  GLuint idx = circles.size();
  circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  stencil_circle_indices.push_back(idx);
}

void PrimitivesCollection::add_stencil_circle(glm::vec2 center, float r) {
  GLuint idx = circles.size();
  circles.push_back({center, r, 0.0, 0.0});
  stencil_circle_indices.push_back(idx);
}

void PrimitivesCollection::add_stencil_segment(glm::vec2 center, float r, float start_angle,
                                               float end_angle) {
  GLuint idx = circles.size();
  circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  stencil_segment_indices.push_back(idx);
}

void PrimitivesCollection::add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
  GLuint idx = vertexes.size();
  vertexes.push_back({p1});
  vertexes.push_back({p2});
  vertexes.push_back({p3});
  stencil_triangle_indices.push_back(idx);
  stencil_triangle_indices.push_back(idx + 1);
  stencil_triangle_indices.push_back(idx + 2);
}

void PrimitivesCollection::add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};
  GLuint idx = vertexes.size();
  vertexes.push_back({top_left});
  vertexes.push_back({bottom_left});
  vertexes.push_back({top_right});
  vertexes.push_back({bottom_right});
  for (uint8_t t : {0, 2, 1, 2, 3, 1}) {
    stencil_triangle_indices.push_back(idx + t);
  }
}

void PrimitivesCollection::update_from(const PrimitivesCollection &other) {
  size_t points_cnt = color_vertexes.size();
  add_elements(points_cnt, line_indices, other.line_indices);
  add_elements(points_cnt, triangle_indices, other.triangle_indices);
  color_vertexes.reserve(points_cnt + other.color_vertexes.size());
  for (auto &obj : other.color_vertexes) {
    color_vertexes.emplace_back(obj);
  }

  points_cnt = vertexes.size();
  add_elements(points_cnt, stencil_triangle_indices, other.stencil_triangle_indices);
  vertexes.reserve(points_cnt + other.vertexes.size());
  for (auto &obj : other.vertexes) {
    vertexes.emplace_back(obj);
  }

  size_t circles_cnt = color_circles.size();
  add_elements(circles_cnt, thin_circle_indices, other.thin_circle_indices);
  add_elements(circles_cnt, filled_circle_indices, other.filled_circle_indices);
  add_elements(circles_cnt, filled_segment_indices, other.filled_segment_indices);
  color_circles.reserve(circles_cnt + other.color_circles.size());
  for (auto &obj : other.color_circles) {
    color_circles.emplace_back(obj);
  }

  circles_cnt = circles.size();
  add_elements(circles_cnt, stencil_circle_indices, other.stencil_circle_indices);
  add_elements(circles_cnt, stencil_segment_indices, other.stencil_segment_indices);
  circles.reserve(circles_cnt + other.circles.size());
  for (auto &obj : other.circles) {
    circles.emplace_back(obj);
  }
}

void PrimitivesCollection::clear() {
  vertexes.clear();
  color_vertexes.clear();
  circles.clear();
  color_circles.clear();
  filled_circle_indices.clear();
  filled_segment_indices.clear();
  thin_circle_indices.clear();
  triangle_indices.clear();
  stencil_triangle_indices.clear();
  stencil_circle_indices.clear();
  stencil_segment_indices.clear();
  line_indices.clear();
}

}  // namespace rewind_viewer::gl

#include "gl/primitives_collection.h"

#include <glm/gtc/constants.hpp>
#include <stdexcept>

namespace {

inline void transfer_induces(size_t shift, std::vector<GLuint> &to, std::vector<GLuint> &from) {
  to.reserve(to.size() + from.size());
  for (GLuint i : from) {
    to.push_back(shift + i);
  }
  from.clear();
}

template <typename T>
inline void transfer(std::vector<T> &v1, std::vector<T> &v2) {
  v1.reserve(v1.size() + v2.size());
  v1.insert(v1.end(), std::make_move_iterator(v2.begin()), std::make_move_iterator(v2.end()));
  v2.clear();
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
                                    uint32_t color, bool fill) {
  GLuint idx = buffer->color_circles.size();
  buffer->color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  if (fill) {
    filled_circles.push_back(idx);
  } else {
    thin_circles.push_back(idx);
  }
}

void PrimitivesCollection::add_circle(glm::vec2 center, float r, uint32_t color, bool fill) {
  GLuint idx = buffer->color_circles.size();
  buffer->color_circles.push_back({color, center, r, 0.0, 0.0});
  if (fill) {
    filled_circles.push_back(idx);
  } else {
    thin_circles.push_back(idx);
  }
}

void PrimitivesCollection::add_segment(glm::vec2 center, float r, float start_angle,
                                        float end_angle, uint32_t color, bool fill) {
  GLuint idx = buffer->color_circles.size();
  buffer->color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  if (fill) {
    filled_segments.push_back(idx);
  } else {
    thin_circles.push_back(idx);  // same logic as circle for shader

    buffer->color_vertexes.push_back({color, center});
    buffer->color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(start_angle), glm::sin(start_angle))});
    buffer->color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(end_angle), glm::sin(end_angle))});

    // Add line between two points in sequence
    GLuint idx = buffer->color_vertexes.size() - 1;
    lines.push_back(idx - 2);
    lines.push_back(idx - 1);
    lines.push_back(idx - 2);
    lines.push_back(idx);
  }
}

void PrimitivesCollection::add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, uint32_t color,
                                         bool fill) {
  GLuint idx = buffer->color_vertexes.size();
  buffer->color_vertexes.push_back({color, p1});
  buffer->color_vertexes.push_back({color, p2});
  buffer->color_vertexes.push_back({color, p3});

  if (fill) {
    triangles.push_back(idx);
    triangles.push_back(idx + 1);
    triangles.push_back(idx + 2);
  } else {
    for (uint8_t t : {0, 1, 1, 2, 2, 0}) {
      lines.push_back(idx + t);
    }
  }
}

void PrimitivesCollection::add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right,
                                          uint32_t color, bool fill) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};
  GLuint idx = buffer->color_vertexes.size();
  buffer->color_vertexes.push_back({color, top_left});
  buffer->color_vertexes.push_back({color, bottom_left});
  buffer->color_vertexes.push_back({color, top_right});
  buffer->color_vertexes.push_back({color, bottom_right});

  if (fill) {
    for (uint8_t t : {0, 2, 1, 2, 3, 1}) {
      triangles.push_back(idx + t);
    }
  } else {
    for (uint8_t t : {0, 2, 2, 3, 3, 1, 1, 0}) {
      lines.push_back(idx + t);
    }
  }
}

void PrimitivesCollection::add_polyline(const std::vector<glm::vec2> &points, uint32_t color) {
  if (points.size() < 2) {
    throw std::invalid_argument("Cannot create polyline from one point");
  }

  buffer->color_vertexes.push_back({color, points[0]});
  for (size_t i = 1; i < points.size(); ++i) {
    buffer->color_vertexes.push_back({color, points[i]});

    // Add line between two points in sequence
    GLuint idx = buffer->color_vertexes.size() - 1;
    lines.push_back(idx - 1);
    lines.push_back(idx);
  }
}

void PrimitivesCollection::add_stencil_arc(glm::vec2 center, float r, float start_angle,
                                            float end_angle) {
  GLuint idx = buffer->circles.size();
  buffer->circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  stencil_circles.push_back(idx);
}

void PrimitivesCollection::add_stencil_circle(glm::vec2 center, float r) {
  GLuint idx = buffer->circles.size();
  buffer->circles.push_back({center, r, 0.0, 0.0});
  stencil_circles.push_back(idx);
}

void PrimitivesCollection::add_stencil_segment(glm::vec2 center, float r, float start_angle,
                                                float end_angle) {
  GLuint idx = buffer->circles.size();
  buffer->circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  stencil_segments.push_back(idx);
}

void PrimitivesCollection::add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
  GLuint idx = buffer->vertexes.size();
  buffer->vertexes.push_back({p1});
  buffer->vertexes.push_back({p2});
  buffer->vertexes.push_back({p3});
  stencil_triangles.push_back(idx);
  stencil_triangles.push_back(idx + 1);
  stencil_triangles.push_back(idx + 2);
}

void PrimitivesCollection::add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};
  GLuint idx = buffer->vertexes.size();
  buffer->vertexes.push_back({top_left});
  buffer->vertexes.push_back({bottom_left});
  buffer->vertexes.push_back({top_right});
  buffer->vertexes.push_back({bottom_right});
  for (uint8_t t : {0, 2, 1, 2, 3, 1}) {
    stencil_triangles.push_back(idx + t);
  }
}

void PrimitivesCollection::transfer_from(PrimitivesCollection &other) {
  size_t points_cnt = buffer->color_vertexes.size();
  transfer_induces(points_cnt, lines, other.lines);
  transfer_induces(points_cnt, triangles, other.triangles);

  points_cnt = buffer->vertexes.size();
  transfer_induces(points_cnt, stencil_triangles, other.stencil_triangles);

  size_t circles_cnt = buffer->color_circles.size();
  transfer_induces(circles_cnt, thin_circles, other.thin_circles);
  transfer_induces(circles_cnt, filled_circles, other.filled_circles);
  transfer_induces(circles_cnt, filled_segments, other.filled_segments);

  circles_cnt = buffer->circles.size();
  transfer_induces(circles_cnt, stencil_circles, other.stencil_circles);
  transfer_induces(circles_cnt, stencil_segments, other.stencil_segments);
}

void PrimitivesStorage::transfer_from(PrimitivesStorage &other) {
  transfer(color_vertexes, other.color_vertexes);
  transfer(vertexes, other.vertexes);
  transfer(color_circles, other.color_circles);
  transfer(circles, other.circles);
}

}  // namespace rewind_viewer::gl

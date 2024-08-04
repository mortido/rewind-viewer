#include "gl/primitives_collection.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace {

inline void transfer_indices(size_t shift, std::vector<GLuint>& to, std::vector<GLuint>& from) {
  to.reserve(to.size() + from.size());
  for (GLuint i : from) {
    to.push_back(shift + i);
  }
  from.clear();
}

template <typename T>
inline void transfer(std::vector<T>& v1, std::vector<T>& v2) {
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

void PrimitiveIndices::add_arc(PrimitiveStorage& storage, glm::vec2 center, float r,
                                float start_angle, float end_angle, uint32_t color, bool fill) {
  GLuint idx = storage.color_circles.size();
  storage.color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  storage.updated = true;
  if (fill) {
    filled_circles.push_back(idx);
  } else {
    thin_circles.push_back(idx);
  }
}

void PrimitiveIndices::add_circle(PrimitiveStorage& storage, glm::vec2 center, float r,
                                   uint32_t color, bool fill) {
  GLuint idx = storage.color_circles.size();
  storage.color_circles.push_back({color, center, r, 0.0, 0.0});
  storage.updated = true;
  if (fill) {
    filled_circles.push_back(idx);
  } else {
    thin_circles.push_back(idx);
  }
}

void PrimitiveIndices::add_segment(PrimitiveStorage& storage, glm::vec2 center, float r,
                                    float start_angle, float end_angle, uint32_t color, bool fill) {
  GLuint idx = storage.color_circles.size();
  storage.color_circles.push_back(
      {color, center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  if (fill) {
    filled_segments.push_back(idx);
  } else {
    thin_circles.push_back(idx);  // same logic as circle for shader

    storage.color_vertexes.push_back({color, center});
    storage.color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(start_angle), glm::sin(start_angle))});
    storage.color_vertexes.push_back(
        {color, center + r * glm::vec2(glm::cos(end_angle), glm::sin(end_angle))});
    storage.updated = true;

    // Add line between two points in sequence
    GLuint idx = storage.color_vertexes.size() - 1;
    lines.push_back(idx - 2);
    lines.push_back(idx - 1);
    lines.push_back(idx - 2);
    lines.push_back(idx);
  }
}

void PrimitiveIndices::add_triangle(PrimitiveStorage& storage, glm::vec2 p1, glm::vec2 p2,
                                     glm::vec2 p3, uint32_t color, bool fill) {
  GLuint idx = storage.color_vertexes.size();
  storage.color_vertexes.push_back({color, p1});
  storage.color_vertexes.push_back({color, p2});
  storage.color_vertexes.push_back({color, p3});
  storage.updated = true;

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

void PrimitiveIndices::add_rectangle(PrimitiveStorage& storage, glm::vec2 top_left,
                                      glm::vec2 bottom_right, uint32_t color, bool fill) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};
  GLuint idx = storage.color_vertexes.size();
  storage.color_vertexes.push_back({color, top_left});
  storage.color_vertexes.push_back({color, bottom_left});
  storage.color_vertexes.push_back({color, top_right});
  storage.color_vertexes.push_back({color, bottom_right});
  storage.updated = true;

//  // Calculate the center of the rectangle
//  glm::vec2 center = (top_left + bottom_right) * 0.5f;
//
//  // Create a 4x4 transformation matrix
//  glm::mat4 transform = glm::mat4(1.0f);
//
//  // Apply translation to move the rectangle to the origin, rotate, and then translate back
//  transform = glm::translate(transform, glm::vec3(center, 0.0f));
//  transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//  transform = glm::translate(transform, glm::vec3(-center, 0.0f));
//
//  // Define the four corners of the rectangle as 4D vectors (homogeneous coordinates)
//  glm::vec4 tl = transform * glm::vec4(top_left, 0.0f, 1.0f);
//  glm::vec4 tr = transform * glm::vec4(bottom_right.x, top_left.y, 0.0f, 1.0f);
//  glm::vec4 bl = transform * glm::vec4(top_left.x, bottom_right.y, 0.0f, 1.0f);
//  glm::vec4 br = transform * glm::vec4(bottom_right, 0.0f, 1.0f);
//
//  // Store the vertices in the storage
//  GLuint idx = storage.color_vertexes.size();
//  storage.color_vertexes.push_back({color, glm::vec2(tl)});
//  storage.color_vertexes.push_back({color, glm::vec2(bl)});
//  storage.color_vertexes.push_back({color, glm::vec2(tr)});
//  storage.color_vertexes.push_back({color, glm::vec2(br)});
//  storage.updated = true;


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

void PrimitiveIndices::add_polyline(PrimitiveStorage& storage,
                                     const std::vector<glm::vec2>& points, uint32_t color) {
  if (points.size() < 2) {
    throw std::invalid_argument("Cannot create polyline from one point");
  }

  storage.color_vertexes.push_back({color, points[0]});
  storage.updated = true;
  for (size_t i = 1; i < points.size(); ++i) {
    storage.color_vertexes.push_back({color, points[i]});

    // Add line between two points in sequence
    GLuint idx = storage.color_vertexes.size() - 1;
    lines.push_back(idx - 1);
    lines.push_back(idx);
  }
}

void PrimitiveIndices::add_stencil_arc(PrimitiveStorage& storage, glm::vec2 center, float r,
                                        float start_angle, float end_angle) {
  GLuint idx = storage.circles.size();
  storage.circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  storage.updated = true;
  stencil_circles.push_back(idx);
}

void PrimitiveIndices::add_stencil_circle(PrimitiveStorage& storage, glm::vec2 center, float r) {
  GLuint idx = storage.circles.size();
  storage.circles.push_back({center, r, 0.0, 0.0});
  storage.updated = true;
  stencil_circles.push_back(idx);
}

void PrimitiveIndices::add_stencil_segment(PrimitiveStorage& storage, glm::vec2 center, float r,
                                            float start_angle, float end_angle) {
  GLuint idx = storage.circles.size();
  storage.circles.push_back({center, r, normalize_angle(start_angle), normalize_angle(end_angle)});
  storage.updated = true;
  stencil_segments.push_back(idx);
}

void PrimitiveIndices::add_stencil_triangle(PrimitiveStorage& storage, glm::vec2 p1, glm::vec2 p2,
                                             glm::vec2 p3) {
  GLuint idx = storage.vertexes.size();
  storage.vertexes.push_back({p1});
  storage.vertexes.push_back({p2});
  storage.vertexes.push_back({p3});
  storage.updated = true;
  stencil_triangles.push_back(idx);
  stencil_triangles.push_back(idx + 1);
  stencil_triangles.push_back(idx + 2);
}

void PrimitiveIndices::add_stencil_rectangle(PrimitiveStorage& storage, glm::vec2 top_left,
                                              glm::vec2 bottom_right) {
  auto top_right = glm::vec2{bottom_right.x, top_left.y};
  auto bottom_left = glm::vec2{top_left.x, bottom_right.y};
  GLuint idx = storage.vertexes.size();
  storage.vertexes.push_back({top_left});
  storage.vertexes.push_back({bottom_left});
  storage.vertexes.push_back({top_right});
  storage.vertexes.push_back({bottom_right});
  storage.updated = true;
  for (uint8_t t : {0, 2, 1, 2, 3, 1}) {
    stencil_triangles.push_back(idx + t);
  }
}

void PrimitiveStorage::transfer_from(PrimitiveStorage& other) {
  transfer(color_vertexes, other.color_vertexes);
  transfer(vertexes, other.vertexes);
  transfer(color_circles, other.color_circles);
  transfer(circles, other.circles);
  updated = true;
}

void PrimitiveIndices::transfer_from(const PrimitiveStorage& storage, PrimitiveIndices& other) {
  size_t points_cnt = storage.color_vertexes.size();
  transfer_indices(points_cnt, lines, other.lines);
  transfer_indices(points_cnt, triangles, other.triangles);

  points_cnt = storage.vertexes.size();
  transfer_indices(points_cnt, stencil_triangles, other.stencil_triangles);

  size_t circles_cnt = storage.color_circles.size();
  transfer_indices(circles_cnt, thin_circles, other.thin_circles);
  transfer_indices(circles_cnt, filled_circles, other.filled_circles);
  transfer_indices(circles_cnt, filled_segments, other.filled_segments);

  circles_cnt = storage.circles.size();
  transfer_indices(circles_cnt, stencil_circles, other.stencil_circles);
  transfer_indices(circles_cnt, stencil_segments, other.stencil_segments);
}

}  // namespace rewind_viewer::gl

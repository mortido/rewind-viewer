#pragma once

#include <glm/glm.hpp>

namespace rewind_viewer::gl {

#pragma pack(push, 1)

struct Vertex {
  uint32_t projection_idx;
  glm::vec2 position;
};

struct ColorVertex {
  uint32_t projection_idx;
  uint32_t color;
  glm::vec2 position;
};

struct Circle {
  uint32_t projection_idx;
  glm::vec2 center;
  float radius;
  float start_angle;
  float end_angle;
};

struct ColorCircle {
  uint32_t projection_idx;
  uint32_t color;
  glm::vec2 center;
  float radius;
  float start_angle;
  float end_angle;
};

#pragma pack(pop)

}  // namespace rewind_viewer::render::gl

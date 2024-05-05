#pragma once

#include <glm/glm.hpp>

namespace rewind_viewer::gl {

#pragma pack(push, 1)

struct Vertex {
  glm::vec2 position;
};

struct ColorVertex {
  glm::vec4 color;
  glm::vec2 position;
};

struct Circle {
  glm::vec2 center;
  float radius;
  float start_angle;
  float end_angle;
};

struct ColorCircle {
  glm::vec4 color;
  glm::vec2 center;
  float radius;
  float start_angle;
  float end_angle;
};

#pragma pack(pop)

}  // namespace rewind_viewer::render::gl

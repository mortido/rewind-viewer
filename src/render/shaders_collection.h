#pragma once

#include "render/Shader.h"

namespace rewind_viewer::render {

struct ShadersCollection {
  ShadersCollection(const std::string& shaders_dir)
      : color_pos(shaders_dir, "color_pos.vert", "color_pos.frag")
      , circle(shaders_dir, "circle.vert", "circle.frag", "circle.geom")
      , color(shaders_dir, "simple.vert", "uniform_color.frag") {}

  Shader color_pos;
  Shader circle;
  // Forward pass vertices with uniform-specified color
  Shader color;
};

}  // namespace rewind_viewer::models

#pragma once

#include <filesystem>

#include "gl/shader.h"

namespace rewind_viewer::gl {

struct ShadersCollection {
  Shader color_pass;
  Shader color_circle;
  Shader uniform_color_circle;
  Shader uniform_color_model;
  Shader uniform_color;

  ShadersCollection(const std::filesystem::path& shaders_dir)
      : color_pass(shaders_dir / "color_pass_projection.vert",
                   shaders_dir / "color_pass.frag")
      , color_circle(shaders_dir / "color_circle.vert",
                     shaders_dir / "color_circle.frag",
                     shaders_dir / "color_circle.geom")
      , uniform_color_circle(shaders_dir / "uniform_color_circle.vert",
                             shaders_dir / "uniform_color_circle.frag",
                             shaders_dir / "uniform_color_circle.geom")
      , uniform_color_model(shaders_dir / "model_view_projection.vert",
                            shaders_dir / "uniform_color.frag")
      , uniform_color(shaders_dir / "simple_projection.vert",
                      shaders_dir / "uniform_color.frag") {}
};

}  // namespace rewind_viewer::gl

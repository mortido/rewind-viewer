#pragma once

#include <glm/vec2.hpp>
#include <string>
#include "models/camera.h"

namespace rewind_viewer::models {

struct Text {
  std::string text;
  glm::vec2 position;
  float size;
  uint32_t color;
  CameraOrigin origin;
};

}  // namespace rewind_viewer::models

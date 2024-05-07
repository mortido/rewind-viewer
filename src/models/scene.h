#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>

#include "common/lock.h"
#include "gl/renderer.h"
#include "models/camera.h"
#include "models/config.h"
#include "models/frames_sequence.h"

namespace rewind_viewer::models {

class Scene {
 public:
  Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode);
  void render(size_t frame_idx);
  void set_canvas_config(const glm::vec2 &position, const glm::vec2 &size,
                         const glm::u16vec2 &grid);

 public:
  models::Camera camera;
  FramesSequence frames;

 private:
  const std::shared_ptr<const SceneConfig> config_;
  gl::Renderer renderer_;
};

}  // namespace rewind_viewer::models

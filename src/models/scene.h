#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>

#include "common/lock.h"
#include "gl/renderer.h"
#include "gl/render_context.h"
#include "gl/resource_manager.h"
#include "models/camera.h"
#include "models/config.h"
#include "models/frame.h"

namespace rewind_viewer::models {

class Scene {
 public:
  Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode);

  void render(size_t frame_idx);
  std::shared_ptr<UIFrame> get_frame(size_t *idx);
  void set_buffered_mode(bool mode);
  size_t frames_count() const;
  void clear();
  void set_canvas_config(const glm::vec2 &position, const glm::vec2 &size,
                         const glm::u16vec2 &grid);
  std::shared_ptr<UIFrame> get_ui_frame();
  std::shared_ptr<Frame> get_draw_frame(bool permanent);
  void commit_frame();

 public:
  models::Camera camera;

 private:
  mutable Spinlock mutex_;
  const std::shared_ptr<const SceneConfig> config_;
  gl::ResourceManager gl_resources_;
  gl::RenderContext frame_context_;
  gl::RenderContext permanent_frame_context_;
  gl::Renderer renderer_;
  bool buffered_mode_;
  std::shared_ptr<UIFrame> buffer_frame_;
  std::shared_ptr<Frame> buffer_permanent_frame_;
  std::shared_ptr<Frame> permanent_frame_;
  std::vector<std::shared_ptr<UIFrame>> frames_;
  size_t prev_rendered_frame_ = 0;
};

}  // namespace rewind_viewer::models

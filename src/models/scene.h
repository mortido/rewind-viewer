#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <mutex>

#include "common/lock.h"
#include "gl/render_context.h"
#include "gl/renderer.h"
#include "gl/resource_manager.h"
#include "models/camera.h"
#include "models/config.h"
#include "models/frame.h"

namespace rewind_viewer::models {

class Scene {
 public:
  Scene(std::shared_ptr<const SceneConfig> config, bool buffered_mode);

  void render();
  void reset();

  size_t get_current_frame_idx() const;
  void set_current_frame_idx(size_t frame_idx);
  size_t frames_count() const;
  std::shared_ptr<Frame> get_current_frame(bool permanent = false);

  std::shared_ptr<Frame> get_buffer_frame(bool permanent);
  void commit_frame();

  void set_layer_name(size_t id, std::string name, bool permanent = false);
  std::string get_layer_name(size_t id, bool permanent = false) const;

  models::Camera &get_camera()  { return camera_; }
  gl::Renderer &get_renderer() { return renderer_; }
  bool get_buffered_mode() const;
  void set_buffered_mode(bool mode);

 private:
  mutable Spinlock mutex_;
  models::Camera camera_;
  const std::shared_ptr<const SceneConfig> config_;
  gl::ResourceManager gl_resources_;
  gl::RenderContext frame_context_;
  gl::RenderContext permanent_frame_context_;
  gl::Renderer renderer_;
  bool buffered_mode_;
  std::shared_ptr<Frame> permanent_frame_;
  std::shared_ptr<Frame> permanent_buffer_frame_;
  std::vector<std::shared_ptr<Frame>> frames_;

  size_t frames_count_ = 0;
  size_t current_frame_ = 0;
  size_t prev_rendered_frame_ = 0;
  std::array<std::string, Frame::LAYERS_COUNT> layers_names_;
  std::array<std::string, Frame::LAYERS_COUNT> permanent_layers_names_;
};

}  // namespace rewind_viewer::models

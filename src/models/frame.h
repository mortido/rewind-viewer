#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "common/lock.h"
#include "common/lock_vector.h"
#include "gl/primitives_collection.h"
#include "gl/render_context.h"
#include "gl/renderer.h"
#include "models/camera.h"
#include "models/popup.h"
#include "models/text.h"

namespace rewind_viewer::models {

class SceneEditor;

class Frame {
  friend class SceneEditor;

 public:
  constexpr static size_t LAYERS_COUNT = 10;
  const std::map<std::string, CameraView> &get_cameras() const;
  const std::string &get_user_message() const;
  const LockVector<Popup>& get_popups(size_t layer) const;
  const LockVector<Text>& get_texts(size_t layer) const;

  void transfer_from(Frame &other);
  void render(const gl::RenderContext &context, gl::Renderer &renderer,
              const std::array<bool, LAYERS_COUNT> &enabled_layers, bool force_load = false);

 private:
  mutable Spinlock mutex_;
  gl::PrimitiveStorage primitives_storage_;
  std::array<gl::PrimitiveIndices, LAYERS_COUNT> primitives_indices_;
  std::array<LockVector<Popup>, LAYERS_COUNT> popups_;
  std::array<LockVector<Text>, LAYERS_COUNT> texts_;
  std::string user_message_;
  std::map<std::string, CameraView> camera_views_;
};

}  // namespace rewind_viewer::models

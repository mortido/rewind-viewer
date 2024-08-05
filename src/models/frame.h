#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "common/lock.h"
#include "gl/primitives_collection.h"
#include "gl/render_context.h"
#include "gl/renderer.h"
#include "models/camera.h"
#include "models/popup.h"

namespace rewind_viewer::models {

class FrameEditor;

class Frame {
  friend class FrameEditor;

 public:
  constexpr static size_t LAYERS_COUNT = 10;

  void transfer_from(Frame &other);
  void render(const gl::RenderContext &context, gl::Renderer &renderer,
              const std::array<bool, LAYERS_COUNT> &enabled_layers, bool force_load = false);

 protected:
  mutable Spinlock mutex_;
  gl::PrimitiveStorage primitives_storage_;
  std::array<gl::PrimitiveIndices, LAYERS_COUNT> primitives_indices_;
};

class UIFrame : public Frame {
  friend class FrameEditor;

 public:
  using PopupCollectionT = std::array<std::vector<Popup>, LAYERS_COUNT>;

  const std::map<std::string, CameraView> &get_cameras() const;
  const std::string &get_user_message() const;
  std::string get_popup_text(glm::vec2 mouse_game_pos,
                             const std::array<bool, LAYERS_COUNT> &enabled_layers);

 protected:
  PopupCollectionT popups_;
  std::string user_message_;
  std::map<std::string, CameraView> camera_views_;
};

}  // namespace rewind_viewer::models

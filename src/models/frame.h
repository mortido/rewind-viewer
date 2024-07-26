#pragma once

#include <array>
#include <map>
#include <memory>
#include <vector>

#include "common/lock.h"
#include "gl/primitives_collection.h"
#include "gl/renderer.h"
#include "models/camera.h"
#include "models/popup.h"

namespace rewind_viewer::models {

class Frame {
 public:
  constexpr static size_t LAYERS_COUNT = 10;
  using PrimitivesT = std::array<gl::PrimitivesCollection, LAYERS_COUNT>;

  void transfer_from(Frame &other);
  const PrimitivesT& all_primitives() const;
  gl::PrimitivesCollection& layer_primitives(size_t layer);

  Frame() : primitives_{std::make_shared<gl::PrimitivesStorage>()} {
    for (size_t i = 0; i < LAYERS_COUNT; i++) {
      contexts_[i].buffer = primitives_;
    }
  }

  void lock() {
    mutex_.lock();
  }

  void unlock() {
    mutex_.unlock();
  }

 protected:
  mutable Spinlock mutex_;
  std::shared_ptr<gl::PrimitivesStorage> primitives_;
  PrimitivesT contexts_;
};

class UIFrame : public Frame {
 public:
  using PopupCollectionT = std::array<std::vector<Popup>, Frame::LAYERS_COUNT>;

  void add_camera_view(const std::string &name, CameraView view);
  const std::map<std::string, CameraView> &get_cameras() const;
  void add_user_text(const std::string &msg);
  const std::string &get_user_message() const;
  void add_box_popup(size_t layer, glm::vec2 center, glm::vec2 size, std::string message);
  void add_round_popup(size_t layer, glm::vec2 center, float radius, std::string message);
  ScopeLockedRefWrapper<const PopupCollectionT, Spinlock> get_popups();

 protected:
  PopupCollectionT popups_;
  std::string user_message_;
  std::map<std::string, CameraView> camera_views_;
};

}  // namespace rewind_viewer::models

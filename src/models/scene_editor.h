#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/lock.h"
#include "gl/primitives_collection.h"
#include "models/scene.h"

#define PRIMITIVE_ADD_METHOD(METHOD_NAME)                                                          \
  template <typename... Args>                                                                      \
  void METHOD_NAME(Args&&... args) {                                                               \
    if (proto_creation_) {                                                                         \
      protos_indices_.back().METHOD_NAME(proto_storage_, 0, std::forward<Args>(args)...);          \
    } else {                                                                                       \
      auto frame = scene_->get_draw_frame(use_permanent_);                                         \
      std::lock_guard lock(frame->mutex_);                                                         \
      frame->primitives_indices_[layer_id_].METHOD_NAME(frame->primitives_storage_, proj_view_id_, \
                                                        std::forward<Args>(args)...);              \
    }                                                                                              \
  }

namespace rewind_viewer::models {

constexpr size_t DEFAULT_LAYER = 2;

class SceneEditor {
 private:
  std::shared_ptr<models::Scene> scene_;
  bool master_;
  bool use_permanent_ = false;
  bool proto_creation_ = false;
  size_t layer_id_ = DEFAULT_LAYER;
  uint32_t proj_view_id_ = 0;
  gl::PrimitiveStorage proto_storage_;
  std::vector<gl::PrimitiveIndices> protos_indices_;

 public:
  SceneEditor(std::shared_ptr<models::Scene> scene, bool master = false)
      : scene_{std::move(scene)}, master_{master} {}

  void reset();
  void set_map(const glm::vec2& position, const glm::vec2& size, const glm::u16vec2& grid);
  void set_layer(size_t l, bool p, CameraOrigin alignment);
  void set_layer_name(size_t l, std::string name, bool p);

  void start_proto();
  void end_proto();
  void add_proto(size_t idx, glm::vec2 position, float angle, uint32_t color, float scale);

  PRIMITIVE_ADD_METHOD(add_arc)
  PRIMITIVE_ADD_METHOD(add_circle)
  PRIMITIVE_ADD_METHOD(add_segment)
  PRIMITIVE_ADD_METHOD(add_polyline)
  PRIMITIVE_ADD_METHOD(add_triangle)
  PRIMITIVE_ADD_METHOD(add_rectangle)
  PRIMITIVE_ADD_METHOD(add_stencil_arc)
  PRIMITIVE_ADD_METHOD(add_stencil_circle)
  PRIMITIVE_ADD_METHOD(add_stencil_segment)
  PRIMITIVE_ADD_METHOD(add_stencil_triangle)
  PRIMITIVE_ADD_METHOD(add_stencil_rectangle)

  void add_camera_view(const std::string& name, CameraView view);
  void add_user_text(const std::string& msg);
  void add_box_popup(glm::vec2 center, glm::vec2 size, std::string message);
  void add_round_popup(glm::vec2 center, float radius, std::string message);
  void finish_frame();
};

}  // namespace rewind_viewer::models

#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <utility>
#include <vector>

#include "common/lock.h"
#include "models/scene.h"

namespace rewind_viewer::models {

constexpr size_t DEFAULT_LAYER = 2;

class FrameEditor {
 private:
  std::shared_ptr<models::Scene> scene_;
  bool master_;
  bool use_permanent_ = false;
  size_t layer_id_ = DEFAULT_LAYER;

 public:
  FrameEditor(std::shared_ptr<models::Scene> scene, bool master = false)
      : scene_{std::move(scene)}, master_{master} {}

  void reset();
  void set_map(const glm::vec2& position, const glm::vec2& size, const glm::u16vec2& grid);
  void set_layer(size_t l, bool p);

  void add_arc(glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_circle(glm::vec2 center, float r, uint32_t color, bool fill);
  void add_segment(glm::vec2 center, float r, float start_angle, float end_angle, uint32_t color, bool fill);
  void add_polyline(const std::vector<glm::vec2> &points, uint32_t color);
  void add_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, uint32_t color, bool fill);
  void add_rectangle(glm::vec2 top_left, glm::vec2 bottom_right, uint32_t color, bool fill);
  void add_stencil_arc(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_circle(glm::vec2 center, float r);
  void add_stencil_segment(glm::vec2 center, float r, float start_angle, float end_angle);
  void add_stencil_triangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
  void add_stencil_rectangle(glm::vec2 top_left, glm::vec2 bottom_right);
  void add_camera_view(const std::string &name, CameraView view);
  void add_user_text(const std::string &msg);
  void add_box_popup(glm::vec2 center, glm::vec2 size, std::string message);
  void add_round_popup(glm::vec2 center, float radius, std::string message);
  void finish_frame();
};

}  // namespace rewind_viewer::models

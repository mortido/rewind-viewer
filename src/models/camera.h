#pragma once

#include <glm/glm.hpp>
#include "common/lock.h"

namespace rewind_viewer::models {

struct CameraView {
  glm::vec2 position;
  glm::vec2 viewport;
};

class Camera {
 public:
  Camera(const glm::vec2& position, float scale, bool y_axis_up);

  void set_viewport_size(const glm::vec2& new_size);
  void move(const glm::vec2& delta);
  void zoom(float zoom_value, glm::vec2 focal_point);
  glm::vec2 screen_to_game(const glm::vec2& screen_coords);
  [[nodiscard]] const glm::mat4& get_projection_matrix() const;
  [[nodiscard]] const glm::vec2& get_position() const;
  void set_position(const glm::vec2& new_position);
  [[nodiscard]] float get_scale() const;
  [[nodiscard]] bool is_y_axis_up() const;
  void set_y_axis_up(bool value);
  void set_scale(float new_scale);
  void set_view(const CameraView& view, bool ignore_viewport=false);
  void update_projection();

 private:
  glm::vec2 position_;  // Camera position in game coordinates
  float scale_;         // Scale of the viewport
  bool y_axis_up_;
  glm::mat4 projection_;     // Projection matrix
  glm::vec2 viewport_size_;  // Size of the viewport (width, height)
  mutable Spinlock mutex_;
};

}  // namespace rewind_viewer::models

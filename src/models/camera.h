#pragma once

#include <glm/glm.hpp>

#include "common/lock.h"

namespace rewind_viewer::models {

struct CameraView {
  glm::vec2 position;
  glm::vec2 viewport;
};

enum class CameraOrigin {
  game,
  left_top,
  left_center,
  left_bottom,
  right_top,
  right_center,
  right_bottom,
  top_center,
  bottom_center,
  COUNT
};

class Camera {
 public:
  Camera(const glm::vec2& position, float scale, bool y_axis_up);

  void set_viewport_size(const glm::vec2& new_size);
  void move(const glm::vec2& delta);
  void zoom(float zoom_value, glm::vec2 focal_point);
  glm::vec2 screen_to_game(const glm::vec2& screen_coords);
  glm::vec2 game_to_screen(const glm::vec2& game_coords, CameraOrigin origin = CameraOrigin::game);
  [[nodiscard]] const std::array<glm::mat4, static_cast<size_t>(CameraOrigin::COUNT)>&
  get_projection_matrices() const;
  [[nodiscard]] const glm::vec2& get_position() const;
  void set_position(const glm::vec2& new_position);
  [[nodiscard]] float get_scale(CameraOrigin origin = CameraOrigin::game) const;
  [[nodiscard]] bool is_y_axis_up() const;
  void set_y_axis_up(bool value);
  void set_scale(float new_scale);
  void set_view(const CameraView& view);
  void update_projections();

 private:
  glm::vec2 position_;  // Camera position in game coordinates
  float scale_;         // Scale of the viewport
  bool y_axis_up_;
  std::array<glm::mat4, static_cast<size_t>(CameraOrigin::COUNT)> projections_;
  glm::vec2 viewport_size_;  // Size of the viewport (width, height)
  mutable Spinlock mutex_;
};

}  // namespace rewind_viewer::models

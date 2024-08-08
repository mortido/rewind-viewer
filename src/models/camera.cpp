#include "models/camera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace rewind_viewer::models {

Camera::Camera(const glm::vec2& position, float scale, bool y_axis_up)
    : position_{position}
    , scale_{scale}
    , y_axis_up_{y_axis_up}
    , viewport_size_{100.0f, 100.0f} {
  update_projections();
}

void Camera::set_viewport_size(const glm::vec2& new_size) {
  std::lock_guard<Spinlock> lock(mutex_);
  viewport_size_ = new_size;
  //    update_projection();
}

void Camera::move(const glm::vec2& delta) {
  std::lock_guard<Spinlock> lock(mutex_);
  glm::vec2 game_delta = delta * scale_;
  if (y_axis_up_) {
    game_delta.y = -game_delta.y;
  }
  position_ += game_delta;
  //    update_projection();
}

void Camera::zoom(float zoom_value, glm::vec2 game_focal_point) {
  std::lock_guard<Spinlock> lock(mutex_);
  float new_scale = scale_ * zoom_value;
  if (new_scale < 0.001f) {
    zoom_value = 0.001f / scale_;
  } else if (new_scale > 100.0f) {
    zoom_value = 100.0f / scale_;
  }

  scale_ *= zoom_value;
  position_ += (game_focal_point - position_) * (1.0f - zoom_value);
  //    update_projection();
}

glm::vec2 Camera::screen_to_game(const glm::vec2& screen_coords) {
  std::lock_guard<Spinlock> lock(mutex_);
  // Normalized device coordinates (NDC)
  glm::vec2 ndc = screen_coords / viewport_size_ - glm::vec2(0.5f, 0.5f);
  if (y_axis_up_) {
    ndc.y = -ndc.y;
  }
  return position_ + (ndc * viewport_size_ * scale_);
}

glm::vec2 Camera::game_to_screen(const glm::vec2& game_coords, CameraOrigin origin) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (origin==CameraOrigin::game) {
    glm::vec2 ndc = (game_coords - position_) / (viewport_size_ * scale_);
    if (y_axis_up_) {
      ndc.y = -ndc.y;
    }
    return (ndc + glm::vec2(0.5f, 0.5f)) * viewport_size_;
  }

  glm::vec2 screen_coords = game_coords;
  if (y_axis_up_) {
    screen_coords.y *= -1.0f;
  }

  switch (origin) {
    case CameraOrigin::left_top:
      break;
    case CameraOrigin::left_center:
      screen_coords += glm::vec2(0.0f, viewport_size_.y * 0.5f);
      break;
    case CameraOrigin::left_bottom:
      screen_coords += glm::vec2(0.0f, viewport_size_.y);
      break;
    case CameraOrigin::right_top:
      screen_coords += glm::vec2(viewport_size_.x, 0.0f);
      break;
    case CameraOrigin::right_center:
      screen_coords += glm::vec2(viewport_size_.x, viewport_size_.y * 0.5f);
      break;
    case CameraOrigin::right_bottom:
      screen_coords += glm::vec2(viewport_size_.x, viewport_size_.y);
      break;
    case CameraOrigin::top_center:
      screen_coords += glm::vec2(viewport_size_.x * 0.5f, 0.0f);
      break;
    case CameraOrigin::bottom_center:
      screen_coords += glm::vec2(viewport_size_.x * 0.5f, viewport_size_.y);
      break;
    default:
      break;
  }

  return screen_coords;
}


const std::array<glm::mat4, static_cast<size_t>(CameraOrigin::COUNT)>& Camera::get_projection_matrices() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return projections_;
}

const glm::vec2& Camera::get_position() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return position_;
}

void Camera::set_position(const glm::vec2& new_position) {
  std::lock_guard<Spinlock> lock(mutex_);
  position_ = new_position;
  //    update_projection();
}

float Camera::get_scale(CameraOrigin origin) const {
  std::lock_guard<Spinlock> lock(mutex_);
  if (origin==CameraOrigin::game) {
    return scale_;
  }
  return 1.0f;
}

bool Camera::is_y_axis_up() const {
  std::lock_guard<Spinlock> lock(mutex_);
  return y_axis_up_;
}

void Camera::set_y_axis_up(bool value) {
  std::lock_guard<Spinlock> lock(mutex_);
  y_axis_up_ = value;
}

void Camera::set_scale(float new_scale) {
  std::lock_guard<Spinlock> lock(mutex_);
  scale_ = new_scale;
  //    update_projection();
}

void Camera::set_view(const CameraView& view) {
  std::lock_guard<Spinlock> lock(mutex_);
  scale_ = std::max(view.viewport.x / viewport_size_.x, view.viewport.y / viewport_size_.y);
  position_ = view.position;
  //    update_projection();
}

void Camera::update_projections() {
  std::lock_guard<Spinlock> lock(mutex_);
  float half_width = viewport_size_.x * 0.5f;
  float half_width_scaled = half_width * scale_;
  float half_height = viewport_size_.y  * 0.5f;
  float half_height_scaled = half_height * scale_;

  if (y_axis_up_) {
    projections_[0] = glm::ortho(position_.x - half_width_scaled, position_.x + half_width_scaled,
                             position_.y - half_height_scaled, position_.y + half_height_scaled);
    projections_[1] = glm::ortho(0.0f, viewport_size_.x, -viewport_size_.y, 0.0f);     // left_top
    projections_[2] = glm::ortho(0.0f, viewport_size_.x, -half_height, half_height);   // left_center
    projections_[3] = glm::ortho(0.0f, viewport_size_.x, 0.0f, viewport_size_.y);      // left_bottom
    projections_[4] = glm::ortho(-viewport_size_.x, 0.0f, -viewport_size_.y, 0.0f);    // right_top
    projections_[5] = glm::ortho(-viewport_size_.x, 0.0f, -half_height, half_height);  // right_center
    projections_[6] = glm::ortho(-viewport_size_.x, 0.0f, 0.0f, viewport_size_.y);     // right_bottom
    projections_[7] = glm::ortho(-half_width, half_width, -viewport_size_.y, 0.0f);    // top_center
    projections_[8] = glm::ortho(-half_width, half_width, 0.0f, viewport_size_.y);     // bottom_center
  } else {
    projections_[0] = glm::ortho(position_.x - half_width_scaled, position_.x + half_width_scaled,
                             position_.y + half_height_scaled, position_.y - half_height_scaled);
    projections_[1] = glm::ortho(0.0f, viewport_size_.x, viewport_size_.y, 0.0f);      // left_top
    projections_[2] = glm::ortho(0.0f, viewport_size_.x, half_height, -half_height);   // left_center
    projections_[3] = glm::ortho(0.0f, viewport_size_.x, 0.0f, -viewport_size_.y);     // left_bottom
    projections_[4] = glm::ortho(-viewport_size_.x, 0.0f, viewport_size_.y, 0.0f);     // right_top
    projections_[5] = glm::ortho(-viewport_size_.x, 0.0f, half_height, -half_height);  // right_center
    projections_[6] = glm::ortho(-viewport_size_.x, 0.0f, 0.0f, -viewport_size_.y);    // right_bottom
    projections_[7] = glm::ortho(-half_width, half_width, viewport_size_.y, 0.0f);     // top_center
    projections_[8] = glm::ortho(-half_width, half_width, 0.0f, -viewport_size_.y);    // bottom_center
  }
}

}  // namespace rewind_viewer::models

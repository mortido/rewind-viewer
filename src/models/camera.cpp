#include "models/camera.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace rewind_viewer::models {

Camera::Camera(const glm::vec2& position, float scale, bool y_axis_up)
    : position_{position}
    , scale_{scale}
    , y_axis_up_{y_axis_up}
    , projection_{}
    , viewport_size_{100.0f, 100.0f} {
  update_projection();
}

void Camera::set_viewport_size(const glm::vec2& new_size) {
  viewport_size_ = new_size;
  //    update_projection();
}

void Camera::move(const glm::vec2& delta) {
  glm::vec2 game_delta = delta * scale_;
  if (y_axis_up_) {
    game_delta.y = -game_delta.y;
  }
  position_ += game_delta;
  //    update_projection();
}

void Camera::zoom(float zoom_value, glm::vec2 focal_point) {
  focal_point = screen_to_game(focal_point);

  float new_scale = scale_ * zoom_value;
  if (new_scale < 0.01f) {
    zoom_value = 0.01f / scale_;
  } else if (new_scale > 100.0f) {
    zoom_value = 100.0f / scale_;
  }

  scale_ *= zoom_value;
  position_ += (focal_point - position_) * (1.0f - zoom_value);
  //    update_projection();
}

glm::vec2 Camera::screen_to_game(const glm::vec2& screen_coords) {
  // Normalized device coordinates (NDC)
  glm::vec2 ndc = screen_coords / viewport_size_ - glm::vec2(0.5f, 0.5f);
  if (y_axis_up_) {
    ndc.y = -ndc.y;
  }
  return position_ + (ndc * viewport_size_ * scale_);
}

const glm::mat4& Camera::get_projection_matrix() const {
  return projection_;
}

const glm::vec2& Camera::get_position() const {
  return position_;
}

void Camera::set_position(const glm::vec2& new_position) {
  position_ = new_position;
  //    update_projection();
}

float Camera::get_scale() const {
  return scale_;
}

bool Camera::is_y_axis_up() const {
  return y_axis_up_;
}

void Camera::set_y_axis_up(bool value) {
  y_axis_up_ = value;
}

void Camera::set_scale(float new_scale) {
  scale_ = new_scale;
  //    update_projection();
}

void Camera::set_view(const CameraView& view, bool ignore_viewport) {
  if (!ignore_viewport) {
    scale_ = std::max(view.viewport.x / viewport_size_.x, view.viewport.y / viewport_size_.y);
  }
  position_ = view.position;
  //    update_projection();
}

void Camera::update_projection() {
  float half_width = viewport_size_.x * scale_ / 2.0f;
  float half_height = viewport_size_.y * scale_ / 2.0f;

  if (y_axis_up_) {
    projection_ = glm::ortho(position_.x - half_width, position_.x + half_width,
                             position_.y - half_height, position_.y + half_height);
  } else {
    projection_ = glm::ortho(position_.x - half_width, position_.x + half_width,
                             position_.y + half_height, position_.y - half_height);
  }
}

}  // namespace rewind_viewer::models

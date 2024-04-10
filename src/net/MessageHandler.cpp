#include "MessageHandler.h"

#include <common/logger.h>

namespace rewind_viewer::net {

MessageHandler::MessageHandler(Scene *scene) : scene_(scene) {}

void MessageHandler::on_new_connection() {
  scene_->clear_data();
  reset_state();
}

void MessageHandler::set_immediate_mode(bool enabled) {
  send_mode_ = enabled ? Mode::IMMEDIATE : Mode::BATCH;
}

void MessageHandler::on_message_processed(bool end_frame) {
  if (send_mode_ == Mode::BATCH && !end_frame) {
    return;
  }

  if (immediate_data_sent_) {
    // Update last frame, because something already sent to it
    scene_->add_frame_data(*frame_);
  } else {
    // Add new frame, nothing was appended to last one
    scene_->add_frame(std::move(frame_));
    frame_ = nullptr;
  }
  immediate_data_sent_ = !end_frame;
  scene_->add_permanent_frame_data(permanent_frame_);

  if (end_frame) {
    reset_state();
  } else {
    if (!frame_) {
      frame_ = std::make_shared<FrameEditor>();
      frame_->set_layer_id(last_layer_id_);
    } else {
      frame_->clear();
    }
    permanent_frame_.clear();
  }
}

FrameEditor &MessageHandler::get_frame_editor() {
  return use_permanent_ ? permanent_frame_ : *frame_;
}

void MessageHandler::use_permanent_frame(bool use) {
  use_permanent_ = use;
}

void MessageHandler::reset_state() {
  permanent_frame_.clear();
  frame_ = std::make_shared<FrameEditor>();
  use_permanent_ = false;
  immediate_data_sent_ = false;
}

void MessageHandler::set_layer(size_t layer) {
  const auto clamped_layer = cg::clamp<size_t>(layer, 0, Frame::LAYERS_COUNT - 1);
  if (layer > static_cast<size_t>(Frame::LAYERS_COUNT)) {
    LOG_ERROR("Incorrect layer id %zu, should be less than %zu. Will use %zu instead", layer,
              static_cast<size_t>(Frame::LAYERS_COUNT), clamped_layer);
  }

  last_layer_id_ = clamped_layer;
  frame_->set_layer_id(last_layer_id_);
  permanent_frame_.set_layer_id(last_layer_id_);
}

void MessageHandler::set_map_config(glm::vec2 size, glm::u16vec2 grid) {
  scene_->set_map_config(size, grid);
}

}  // namespace rewind_viewer::net

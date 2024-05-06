#include "models/frames_sequence.h"

namespace rewind_viewer::models {

FramesSequence::FramesSequence(bool buffered_mode)
    : buffered_mode_{buffered_mode}
    , buffer_frame_{std::make_shared<UIFrame>()}
    , buffer_permanent_frame_{std::make_shared<Frame>()}
    , permanent_frame_{std::make_shared<Frame>()} {}

size_t FramesSequence::size() const {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    return frames_.size();
  } else {
    return frames_.size() + 1;
  }
}

FramesSequence::FramePairT FramesSequence::get_frame(size_t *idx) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    if (frames_.empty()) {
      return std::make_pair(nullptr, nullptr);
    }
    *idx = std::min(*idx, frames_.size() - 1ul);
  } else {
    *idx = std::min(*idx, frames_.size());
    if (*idx == frames_.size()) {
      return std::make_pair(permanent_frame_, buffer_frame_);
    }
  }
  return std::make_pair(permanent_frame_, frames_[*idx]);
}

void FramesSequence::set_buffered_mode(bool mode) {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_ == mode) {
    return;
  }
  buffered_mode_ = mode;
  if (!buffered_mode_) {
    permanent_frame_->transfer_from(*buffer_permanent_frame_);
  }
}

void FramesSequence::clear() {
  std::lock_guard<Spinlock> lock(mutex_);
  frames_.clear();
  buffer_frame_ = std::make_shared<UIFrame>();
  permanent_frame_ = std::make_shared<Frame>();
  buffer_permanent_frame_ = std::make_shared<Frame>();
}

FramesSequence::FramePairT FramesSequence::get_working_frame() {
  std::lock_guard<Spinlock> lock(mutex_);
  if (buffered_mode_) {
    return std::make_pair(buffer_permanent_frame_, buffer_frame_);
  } else {
    return std::make_pair(permanent_frame_, buffer_frame_);
  }
}

void FramesSequence::commit_frame() {
  std::lock_guard<Spinlock> lock(mutex_);
  frames_.emplace_back(std::move(buffer_frame_));
  buffer_frame_ = std::make_shared<UIFrame>();
  permanent_frame_->transfer_from(*buffer_permanent_frame_);
}

}  // namespace rewind_viewer::models

#pragma once

#include <memory>
#include <mutex>

#include "common/lock.h"
#include "models/frame.h"

namespace rewind_viewer::models {

class Film {
 public:
  using FramePairT = std::pair<std::shared_ptr<Frame>, std::shared_ptr<UIFrame>>;

  Film(bool buffered_mode)
      : buffered_mode_{buffered_mode}
      , buffer_frame_{std::make_shared<UIFrame>()}
      , buffer_permanent_frame_{std::make_shared<Frame>()}
      , permanent_frame_{std::make_shared<Frame>()} {}

  /* vvv Called from UI thread vvv */

  size_t size() const {
    std::lock_guard<Spinlock> lock(mutex_);
    if (buffered_mode_) {
      return frames_.size();
    } else {
      return frames_.size() + 1;
    }
  }

  FramePairT get_frame(size_t* idx) {
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

  void set_buffered_mode(bool mode) {
    std::lock_guard<Spinlock> lock(mutex_);
    if (buffered_mode_ == mode) {
      return;
    }
    buffered_mode_ = mode;
    if (!buffered_mode_) {
      permanent_frame_->transfer_from(*buffer_permanent_frame_);
    }
  }

  /* vvv Called from Network and UI threads vvv */

  void clear() {
    std::lock_guard<Spinlock> lock(mutex_);
    frames_.clear();
    buffer_frame_ = std::make_shared<UIFrame>();
    permanent_frame_ = std::make_shared<Frame>();
    buffer_permanent_frame_ = std::make_shared<Frame>();
  }

  /* vvv Called from Network threads vvv */
  FramePairT get_working_frame() {
    std::lock_guard<Spinlock> lock(mutex_);
    if (buffered_mode_) {
      return std::make_pair(buffer_permanent_frame_, buffer_frame_);
    } else {
      return std::make_pair(permanent_frame_, buffer_frame_);
    }
  }

  void commit_frame() {
    std::lock_guard<Spinlock> lock(mutex_);
    frames_.emplace_back(std::move(buffer_frame_));
    buffer_frame_ = std::make_shared<UIFrame>();
    permanent_frame_->transfer_from(*buffer_permanent_frame_);
  }

 private:
  mutable Spinlock mutex_;

  bool buffered_mode_;
  std::shared_ptr<UIFrame> buffer_frame_;
  std::shared_ptr<Frame> buffer_permanent_frame_;
  std::shared_ptr<Frame> permanent_frame_;
  std::vector<std::shared_ptr<UIFrame>> frames_;
};

}  // namespace rewind_viewer::models

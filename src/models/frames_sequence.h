#pragma once

#include <memory>
#include <mutex>

#include "common/lock.h"
#include "models/frame.h"

namespace rewind_viewer::models {

class FramesSequence {
 public:
  using FramePairT = std::pair<std::shared_ptr<Frame>, std::shared_ptr<UIFrame>>;

  explicit FramesSequence(bool buffered_mode);

  /* vvv Called from UI thread vvv */
  size_t size() const;
  FramePairT get_frame(size_t* idx);
  void set_buffered_mode(bool mode);

  /* vvv Called from Network and UI(removed?) threads vvv */
  void clear();

  /* vvv Called from Network threads vvv */
  FramePairT get_working_frame();
  void commit_frame();

 private:
  mutable Spinlock mutex_;

  bool buffered_mode_;
  std::shared_ptr<UIFrame> buffer_frame_;
  std::shared_ptr<Frame> buffer_permanent_frame_;
  std::shared_ptr<Frame> permanent_frame_;
  std::vector<std::shared_ptr<UIFrame>> frames_;
};

}  // namespace rewind_viewer::models

#pragma once
#include "models/frame_editor.h"
#include "gateway/transport/transport.h"
#include "gateway/events.h"
#include "gateway/lock_dictionary.h"
#include "gateway/actions.h"

namespace rewind_viewer::gateway {

struct ParsingError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

class MessageHandler {
 public:
  virtual ~MessageHandler() = default;
  virtual void handle_message(const uint8_t* buffer, uint32_t size, LockDictionary<char, std::unique_ptr<Event>> & events, LockDictionary<std::string, std::unique_ptr<Action>> & actions,
                              models::FrameEditor& frame_editor, Transport& transport) = 0;

 protected:
  static void normalize_AABB(glm::vec2& min_corner, glm::vec2& max_corner) {
    if (min_corner.x > max_corner.x) {
      std::swap(min_corner.x, max_corner.x);
    }
    if (min_corner.y > max_corner.y) {
      std::swap(min_corner.y, max_corner.y);
    }
  }
};

}  // namespace rewind_viewer::gateway

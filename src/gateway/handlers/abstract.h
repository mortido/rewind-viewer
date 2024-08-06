#pragma once
#include <memory>
#include <utility>

#include "common/lock_dictionary.h"
#include "gateway/actions.h"
#include "gateway/events.h"
#include "gateway/transport/transport.h"
#include "models/scene_editor.h"

namespace rewind_viewer::gateway {

struct ParsingError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

class MessageHandler {
 protected:
  models::SceneEditor& scene_editor_;
  std::shared_ptr<Transport> transport_;
  std::vector<uint8_t> read_buffer_;

 public:
  MessageHandler(models::SceneEditor& scene_editor, std::shared_ptr<Transport> transport)
      : scene_editor_{scene_editor}
      , transport_{std::move(transport)}
      , read_buffer_(MAX_MESSAGE_SIZE + 1) {}

  virtual ~MessageHandler() { transport_->close_connection(); }

  bool next_message(LockDictionary<char, std::unique_ptr<Event>>& events,
                    LockDictionary<std::string, std::unique_ptr<Action>>& actions) {
    if (transport_->is_connected()) {
      uint32_t bytes = transport_->read_msg(read_buffer_.data(), read_buffer_.size());
      handle_message(bytes, events, actions);
      return true;
    }
    return false;
  }

 protected:
  virtual void handle_message(uint32_t size, LockDictionary<char, std::unique_ptr<Event>>& events,
                              LockDictionary<std::string, std::unique_ptr<Action>>& actions) = 0;

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

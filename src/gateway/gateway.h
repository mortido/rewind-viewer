#pragma once

#include <map>
#include <memory>
#include <utility>

#include "gateway/events.h"
#include "gateway/handlers/flatbuffers.h"
#include "gateway/handlers/json.h"
#include "models/frame_editor.h"
#include "models/scene.h"

namespace rewind_viewer::gateway {

constexpr uint16_t FBS_MESSAGE_SCHEMA_VERSION = 7;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

class Gateway {
 public:
  enum class State { wait, connected, closed };

 protected:
  models::FrameEditor frame_editor_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::map<uint16_t, std::unique_ptr<MessageHandler>> message_handlers_;

 public:
  Gateway(std::shared_ptr<models::Scene> scene, bool master = false)
      : frame_editor_(std::move(scene), master)
      , read_buffer_(MAX_MESSAGE_SIZE)
      , state_{State::wait} {
    message_handlers_[JSON_MESSAGE_SCHEMA_VERSION] = std::make_unique<JsonMessageHandler>();
    message_handlers_[FBS_MESSAGE_SCHEMA_VERSION] = std::make_unique<FlatbuffersMessageHandler>();
  }

  virtual ~Gateway() = default;

  Gateway::State get_state() const {
    return state_.load(std::memory_order_relaxed);
  }

  virtual const std::string& get_name() const = 0;
};

}  // namespace rewind_viewer::gateway

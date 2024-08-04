#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "common/lock.h"
#include "gateway/actions.h"
#include "gateway/events.h"
#include "gateway/handlers/flatbuffers.h"
#include "gateway/handlers/json.h"
#include "gateway/lock_dictionary.h"
#include "gateway/transport/transport.h"
#include "models/frame_editor.h"
#include "models/scene.h"

namespace rewind_viewer::gateway {

constexpr uint16_t FBS_MESSAGE_SCHEMA_VERSION = 7;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB
constexpr int RETRY_TIMEOUT_MS = 250;

class ClientGateway {
 public:
  enum class State { wait, established, aborting, closed };

  ClientGateway(std::shared_ptr<models::Scene> scene, std::shared_ptr<ReusableTransport> transport,
                bool master = false);
  ~ClientGateway();

  void stop() { state_ = State::closed; }
  State get_state() const { return state_.load(std::memory_order_relaxed); }
  LockDictionary<char, std::unique_ptr<Event>>& get_events() { return events_; }
  LockDictionary<std::string, std::unique_ptr<Action>>& get_actions() { return actions_; }

  const std::string& get_name() const {
    std::lock_guard lock(mutex_);
    return name_;
  }

  void substitute_transport(std::shared_ptr<Transport> transport) {
    {
      std::lock_guard lock(mutex_);
      one_use_transport_ = std::move(transport);
    }
    // Disconnect current transport
    State expected = State::established;
    state_.compare_exchange_strong(expected, State::aborting);
  }

 private:
  models::FrameEditor frame_editor_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::string name_;
  std::thread transport_thread_;
  std::map<uint16_t, std::unique_ptr<MessageHandler>> message_handlers_;
  LockDictionary<char, std::unique_ptr<Event>> events_;
  LockDictionary<std::string, std::unique_ptr<Action>> actions_;
  mutable Spinlock mutex_;
  std::shared_ptr<ReusableTransport> default_transport_;
  std::shared_ptr<Transport> one_use_transport_;
  std::shared_ptr<Transport> active_transport_;

  void transport_loop();
};

}  // namespace rewind_viewer::gateway

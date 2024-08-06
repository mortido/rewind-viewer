#pragma once

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include "common/lock.h"
#include "common/lock_dictionary.h"
#include "gateway/actions.h"
#include "gateway/events.h"
#include "gateway/handlers/flatbuffers.h"
#include "gateway/handlers/json.h"
#include "gateway/transport/transport.h"
#include "models/scene.h"
#include "models/scene_editor.h"

namespace rewind_viewer::gateway {

constexpr uint16_t FBS_MESSAGE_SCHEMA_VERSION = 7;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
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
  models::SceneEditor scene_editor_;
  std::atomic<State> state_;
  std::string name_;
  std::thread transport_thread_;
  LockDictionary<char, std::unique_ptr<Event>> events_;
  LockDictionary<std::string, std::unique_ptr<Action>> actions_;
  mutable Spinlock mutex_;
  std::shared_ptr<ReusableTransport> default_transport_;
  std::shared_ptr<Transport> one_use_transport_;

  std::unique_ptr<MessageHandler> create_message_handler(std::shared_ptr<Transport> transport);
  std::unique_ptr<MessageHandler> accept_connection();
  void transport_loop();
};

}  // namespace rewind_viewer::gateway

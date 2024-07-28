#pragma once

#include <map>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "models/frame_editor.h"
#include "models/scene.h"
#include "gateway/events.h"
#include "gateway/handlers/flatbuffers.h"
#include "gateway/handlers/json.h"
#include "gateway/tcp_server.h"

namespace rewind_viewer::gateway {

constexpr uint16_t FBS_MESSAGE_SCHEMA_VERSION = 7;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

class ClientGateway {
 public:
  enum class State { wait, established, closed };

  ClientGateway(std::shared_ptr<models::Scene> scene, const std::string& address, uint16_t port,
                bool master = false);

  ~ClientGateway();
  void stop();

  ClientGateway::State get_state() const {
    return state_.load(std::memory_order_relaxed);
  }

  uint16_t get_port() const {
    return tcp_server_.get_port();
  }

  EventsCollection& get_events() {
    return events_;
  }

 private:
  models::FrameEditor frame_editor_;
  gateway::TcpServer tcp_server_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::thread network_thread_;
  std::map<uint16_t, std::unique_ptr<MessageHandler>> message_handlers_;
  EventsCollection events_;

  void network_loop();
};

}  // namespace rewind_viewer::gateway

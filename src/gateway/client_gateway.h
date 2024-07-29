#pragma once

#include <map>
#include <memory>
#include <string>
#include <filesystem>
#include <thread>
#include <utility>
#include <vector>

#include "gateway/events.h"
#include "gateway/handlers/flatbuffers.h"
#include "gateway/handlers/json.h"
#include "gateway/transport/tcp_server.h"
#include "models/frame_editor.h"
#include "models/scene.h"

namespace rewind_viewer::gateway {

constexpr uint16_t FBS_MESSAGE_SCHEMA_VERSION = 7;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

class ClientGateway {
 public:
  enum class State { wait, established, closed };
  enum class Source { none, file, tcp };

  ClientGateway(std::shared_ptr<models::Scene> scene, const std::string& address, uint16_t port,
                bool master = false);

  ~ClientGateway();
  void stop();

  State get_state() const {
    return state_.load(std::memory_order_relaxed);
  }

  const std::string& get_name() const {
    //todo: lock
    return name_;
  }

  Source get_source() const {
    return source_.load(std::memory_order_relaxed);
  }

  EventsCollection& get_events() {
    return events_;
  }

  void read_file(const std::filesystem::path &filename);
  void save_to_file(const std::filesystem::path &filename);

 private:
  std::string name_;
  models::FrameEditor frame_editor_;
  gateway::TcpServer tcp_server_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::atomic<Source> source_;
  std::thread network_thread_;
  std::map<uint16_t, std::unique_ptr<MessageHandler>> message_handlers_;
  EventsCollection events_;

  void transport_loop();
};

}  // namespace rewind_viewer::gateway

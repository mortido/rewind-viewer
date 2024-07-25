#pragma once

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>

#include <map>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "common/lock.h"
#include "models/scene.h"
#include "net/events.h"
#include "net/messages/rewind_event.fbs.h"
#include "net/messages/rewind_message.fbs.h"
#include "net/tcp_server.h"

namespace rewind_viewer::net {

struct ParsingError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 5;
constexpr uint16_t JSON_MESSAGE_SCHEMA_VERSION = 6;
constexpr uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB
constexpr size_t DEFAULT_LAYER = 2ul;

class RewindServer {
  using EventsCollection = std::map<char, std::unique_ptr<net::Event>>;

 public:
  enum class State { wait, established, closed };

  RewindServer(std::shared_ptr<models::Scene> scene, const std::string& address, uint16_t port,
               bool master = false);

  ~RewindServer();
  void stop();

  RewindServer::State get_state() const {
    return state_.load(std::memory_order_relaxed);
  }
  uint16_t get_port() const {
    return tcp_server_.get_port();
  }

  ScopeLockedRefWrapper<EventsCollection, Spinlock> get_events() {
    return {events_, events_mutex_};
  }

 private:
  std::shared_ptr<models::Scene> scene_;
  bool master_;
  bool use_permanent_ = false;
  size_t layer_id_ = DEFAULT_LAYER;

  // todo: atomic if gonna be used by many threads
  uint16_t schema_version_ = 0;

  net::TcpServer tcp_server_;
  flatbuffers::FlatBufferBuilder fbs_builder_;
  rapidjson::StringBuffer json_buffer_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::thread network_thread_;
  EventsCollection events_;
  mutable Spinlock events_mutex_;

  void network_loop();
  void reset();
  void handle_message(const fbs::RewindMessage* message);
  void handle_message(const rapidjson::Document& doc);
};

}  // namespace rewind_viewer::net

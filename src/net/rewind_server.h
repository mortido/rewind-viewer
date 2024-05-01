#pragma once

#include <memory>
#include <thread>
#include <utility>

#include "models/scene.h"
#include "net/messages/rewind_message.fbs.h"
#include "net/tcp_server.h"

namespace rewind_viewer::net {

struct ParsingError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

class RewindServer {
 public:
  constexpr static uint64_t MAX_MESSAGE_SIZE = 1 * 1024;
  constexpr static size_t DEFAULT_LAYER = 2ul;
  enum class State { wait, established, closed };

  RewindServer(std::shared_ptr<models::Scene> scene, const std::string& address, uint16_t port,
               bool master = false);

  ~RewindServer();
  void stop();
  State get_state() const;
  uint16_t get_port() const;

 private:
  std::shared_ptr<models::Scene> scene_;
  bool master_;
  bool use_permanent_ = false;
  size_t layer_id_ = DEFAULT_LAYER;

  net::TcpServer tcp_server_;
  flatbuffers::FlatBufferBuilder builder_;
  std::vector<uint8_t> read_buffer_;
  std::atomic<State> state_;
  std::thread network_thread_;

  void network_loop();
  void reset();
  void handle_message(const fbs::RewindMessage* message);
};

}  // namespace rewind_viewer::net

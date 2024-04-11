#pragma once
#include <clsocket/ActiveSocket.h>
#include <clsocket/PassiveSocket.h>

#include <atomic>

#include "MessageHandler.h"
#include "viewer/Scene.h"

namespace rewind_viewer::net {

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 4;

/**
 * Negotiation with running strategy
 *  - listen socket, read json primitives
 *  - decode json primitives to one of Primitives.h
 *  - configure Frame object
 *  - send Frame to Scene when ready
 *  - running in personal thread
 */
class NetListener {
 public:
  enum class ConStatus { WAIT, ESTABLISHED, CLOSED };

  NetListener(std::string listen_host, uint16_t listen_port,
              std::unique_ptr<MessageHandler> &&handler);

  /// Return current connection status.
  /// Will be wait until first data chunk, established while tcp connection alive
  ConStatus connection_status() const;

  /// Start gathering and operating information from socket
  /// Blocking call, should be running on personal thread
  void run();

  /// Immediate mode
  /// Send primitives as soon as they come, do not wait 'end'
  /// Called from render thread
  void set_immediate_mode(bool enable);

  void stop();

 private:
  void accept_client();
  void serve_connection();
  bool read_bytes(std::string &buf, uint16_t size);

  std::unique_ptr<CPassiveSocket> socket_;
  std::unique_ptr<CActiveSocket> client_;
  ConStatus status_;

  std::string host_;
  uint16_t port_;

  std::unique_ptr<MessageHandler> handler_;

  std::atomic<bool> stop_{false};
  std::atomic<bool> immediate_mode_{false};

  bool is_little_endian_;
};

}  // namespace rewind_viewer::net

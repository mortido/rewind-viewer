#include "NetListener.h"

#include <common/logger.h>

#ifdef __APPLE__
#include <cerrno>
#include <utility>
#endif

namespace rewind_viewer::net {

NetListener::NetListener(std::string listen_host, uint16_t listen_port,
                         std::unique_ptr<MessageHandler> &&handler)
    : host_(std::move(listen_host)), port_(listen_port), handler_(std::move(handler)) {
  socket_ = std::make_unique<CPassiveSocket>(CPassiveSocket::SocketTypeTcp);
  socket_->DisableNagleAlgoritm();
  socket_->Initialize();
  if (!socket_->Listen(host_.data(), port_)) {
    LOG_ERROR("NetListener:: Cannot listen on socket: %d", errno);
  }
  status_ = ConStatus::CLOSED;
}

NetListener::ConStatus NetListener::connection_status() const {
  return status_;
}

void NetListener::run() {
  while (!stop_) {
    status_ = ConStatus::WAIT;
    LOG_INFO("Start listening");
    accept_client();
    LOG_INFO("Got connection from %s:%u", client_->GetClientAddr(),
             client_->GetClientPort());
    status_ = ConStatus::ESTABLISHED;
    serve_connection();
    status_ = ConStatus::CLOSED;
    client_.reset(nullptr);
  }
}

void NetListener::set_immediate_mode(bool enable) {
  immediate_mode_.store(enable);
}

void NetListener::stop() {
  if (status_ != ConStatus::CLOSED) {
    LOG_INFO("Stopping network listening");
  }
  stop_ = true;
}

void NetListener::accept_client() {
  if (!socket_->IsSocketValid()) {
    status_ = ConStatus::CLOSED;
    char buf[256];
    snprintf(buf, sizeof(buf), "Can't accept connection, invalid socket: %s",
             socket_->DescribeError());
    throw std::runtime_error(buf);
  }
  client_.reset(socket_->Accept());
  if (!client_) {
    status_ = ConStatus::CLOSED;
    char buf[256];
    snprintf(buf, sizeof(buf), "Accept on socket returned NULL. errno=%d; %s", errno,
             strerror(errno));
    throw std::runtime_error(buf);
  }
}

void NetListener::serve_connection() {
  // Cleanup previous data
  handler_->on_new_connection();

  // Check schema version on new connection.
  std::string buffer(1024, '\0');
  uint16_t schema_version;
  if (!read_bytes(buffer, sizeof(uint16_t))) {
    return;
  }
  // TODO: endianness
  memcpy(&schema_version, buffer.data(), sizeof(uint16_t));
  if (schema_version != MESSAGE_SCHEMA_VERSION) {
    LOG_ERROR("Incorrect schema version, got %u, required %u", schema_version,
              MESSAGE_SCHEMA_VERSION);
    return;
  } else {
    LOG_INFO("Schema version %u", schema_version);
  }

  uint16_t message_size;
  while (!stop_) {
    if (!read_bytes(buffer, sizeof(uint16_t))) {
      break;
    }
    // TODO: endianness
    memcpy(&message_size, buffer.data(), sizeof(uint16_t));
    LOG_V9("Message %u bytes", message_size);

    if (stop_ || !read_bytes(buffer, message_size)) {
      break;
    }
    handler_->set_immediate_mode(immediate_mode_.load());
    try {
      handler_->handle_message(reinterpret_cast<const uint8_t *>(buffer.data()), message_size);
    } catch (const std::exception &e) {
      LOG_ERROR("handle_message exception: %s", e.what());
    }
  }
}

bool NetListener::read_bytes(std::string &buf, uint16_t size) {
  int32_t i = 0;
  while (size > 0) {
    const int32_t received = client_->Receive(size, reinterpret_cast<uint8_t *>(&buf[i]));
    if (received > 0) {
      i += received;
      size -= received;
    } else {
      LOG_WARN("Client closed");
      return false;
    }
  }
  return i > 0;
}

}  // namespace rewind_viewer::net

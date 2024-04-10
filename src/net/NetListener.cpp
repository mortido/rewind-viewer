#include "NetListener.h"

#include <common/logger.h>

#ifdef __APPLE__
#include <cerrno>
#include <utility>
#endif

namespace {

int32_t read_bytes(CActiveSocket *socket, std::string &buf, uint32_t size) {
  int32_t i = 0;
  while (size > 0) {
    const int32_t received = socket->Receive(size, reinterpret_cast<uint8_t *>(&buf[i]));
    if (received > 0) {
      i += received;
      size -= received;
    } else {
      return 0;
    }
  }
  return i;
}

}  // namespace

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
    LOG_INFO("NetClient:: Start listening");
    if (!socket_->IsSocketValid()) {
      status_ = ConStatus::CLOSED;
      char buf[256];
      snprintf(buf, sizeof(buf), "Can't accept connection, invalid socket: %s",
               socket_->DescribeError());
      throw std::runtime_error(buf);
    }
    std::unique_ptr<CActiveSocket> client_socket(socket_->Accept());
    if (!client_socket) {
      if (stop_) {
        return;
      }

      status_ = ConStatus::CLOSED;
      char buf[256];
      snprintf(buf, sizeof(buf), "Accept on socket returned NULL. errno=%d; %s", errno,
               strerror(errno));
      throw std::runtime_error(buf);
    } else {
      LOG_INFO("NetListener:: Got connection from %s:%u", client_socket->GetClientAddr(),
               static_cast<uint16_t>(client_socket->GetClientPort()));
    }
    status_ = ConStatus::ESTABLISHED;
    // Cleanup previous data
    handler_->on_new_connection();
    // Serve socket
    serve_connection(client_socket.get());
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

void NetListener::serve_connection(CActiveSocket *client) {
  std::string buffer(1024, '\0');
  uint16_t message_size;
  uint16_t schema_version;
  while (!stop_) {
    // TODO: read schema_version
    // TODO: endianness?

    if (read_bytes(client, buffer, sizeof(uint16_t)) == 0) {
      client->Close();
      status_ = ConStatus::CLOSED;
      break;
    }
    memcpy(&message_size, buffer.data(), sizeof(uint16_t));
    if (stop_) {
      break;
    }
    LOG_V9("NetClient:: Message %d bytes", message_size);
    if (read_bytes(client, buffer, message_size) == 0) {
      client->Close();
      status_ = ConStatus::CLOSED;
      break;
    }

    handler_->set_immediate_mode(immediate_mode_.load());
    // Strategy can send several messages in one block
    try {
      handler_->handle_message(reinterpret_cast<const uint8_t *>(buffer.data()), message_size);
    } catch (const std::exception &e) {
      LOG_WARN("NetListener(handler_)::Exception: %s", e.what());
    }
  }
}

}  // namespace rewind_viewer::net

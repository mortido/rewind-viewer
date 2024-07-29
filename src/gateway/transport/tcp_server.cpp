#include "gateway/transport/tcp_server.h"

#include <algorithm>

#include "common/logger.h"

namespace rewind_viewer::gateway {

void TcpServer::read_bytes(uint8_t *buffer, uint32_t size) {
  if (!client_) {
    throw std::runtime_error("Can't read bytes if client is not connected.");
  }
  if (!client_->IsSocketValid()) {
    throw std::runtime_error(
        str_format("Can't read bytes, invalid socket: %s", client_->DescribeError()));
  }
  int pos = 0;
  while (size > 0) {
    int received = client_->Receive(static_cast<int32_t>(size), buffer + pos);
    if (received > 0) {
      pos += received;
      size -= received;
    } else {
      auto error = str_format("Can't read bytes: %s", client_->DescribeError());
      client_.reset(nullptr);
      throw std::runtime_error(error);
    }
  }
}

void TcpServer::send_bytes(const uint8_t *buffer, uint32_t size) {
  if (!client_) {
    throw std::runtime_error("Can't send bytes if client is not connected.");
  }
  if (!client_->IsSocketValid()) {
    throw std::runtime_error(
        str_format("Can not send message, invalid socket: %s", client_->DescribeError()));
  }
  if (size == 0) {
    throw std::runtime_error("Can't send 0 bytes.");
  }

  if (client_->Send(buffer, size) == 0) {
    throw std::runtime_error(str_format("Error sending bytes: %s", client_->DescribeError()));
  }
}

TcpServer::TcpServer(std::string address, uint16_t port)
    : address_(std::move(address))
    , port_(port)
    , socket_(std::make_unique<CPassiveSocket>(CPassiveSocket::SocketTypeTcp)) {}

void TcpServer::initialize() {
  socket_->DisableNagleAlgoritm();
  socket_->Initialize();
  if (!socket_->Listen(address_.c_str(), port_)) {
    throw std::runtime_error(
        str_format("Can't listen to the socket: %s", socket_->DescribeError()));
  }
  LOG_INFO("Start listening on %s:%u", address_.c_str(), port_);
}

bool TcpServer::accept_connection(int timeout) {
  if (!socket_->IsSocketValid()) {
    throw std::runtime_error(
        str_format("Can't accept connection, invalid socket: %s", socket_->DescribeError()));
  }

  if (timeout > 0 && !socket_->Select(timeout, 0)) {
    return false;
  }

  client_.reset(socket_->Accept());
  if (client_ == nullptr) {
    //      LOG_ERROR("Got null connection, continue listening on %s:%u", address_.c_str(), port_);
    LOG_ERROR("Got null connection");
    return false;
  }

  LOG_INFO("Got connection from %s:%u", client_->GetClientAddr(), client_->GetClientPort());
  return true;
}

void TcpServer::discard_connection() {
  client_.reset(nullptr);
}

}  // namespace rewind_viewer::gateway

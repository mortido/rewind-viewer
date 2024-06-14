#include "net/tcp_server.h"

#include <algorithm>

#include "common/logger.h"
#include "common/utils.h"

namespace {

template <typename T>
void swap_bytes(T &value) {
  auto *bytes = reinterpret_cast<uint8_t *>(&value);
  std::reverse(bytes, bytes + sizeof(T));
}

}  // namespace

namespace rewind_viewer::net {

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

TcpServer::TcpServer(std::string address, uint16_t port)
    : address_(std::move(address))
    , port_(port)
    , socket_(std::make_unique<CPassiveSocket>(CPassiveSocket::SocketTypeTcp)) {
  // Could be std::endian::native == std::endian::little in c++20
  const int32_t value{0x01};
  const void *pointer{static_cast<const void *>(&value)};
  const unsigned char *least_significant_address{static_cast<const unsigned char *>(pointer)};
  is_little_endian_ = *least_significant_address == 0x01;
}

void TcpServer::initialize() {
  socket_->DisableNagleAlgoritm();
  socket_->Initialize();
  if (!socket_->Listen(address_.c_str(), port_)) {
    throw std::runtime_error(
        str_format("Can't listen to the socket: %s", socket_->DescribeError()));
  }
  LOG_INFO("Start listening on %s:%u", address_.c_str(), port_);
}

uint16_t TcpServer::accept_connection() {
  if (!socket_->IsSocketValid()) {
    throw std::runtime_error(
        str_format("Can't accept connection, invalid socket: %s", socket_->DescribeError()));
  }
  client_.reset(socket_->Accept());
  if (client_ == nullptr) {
    //      LOG_ERROR("Got null connection, continue listening on %s:%u", address_.c_str(), port_);
    LOG_ERROR("Got null connection");
    return 0;
  }

  LOG_INFO("Got connection from %s:%u", client_->GetClientAddr(), client_->GetClientPort());

  // Check schema version on new connection.
  uint16_t schema_version;
  read_bytes(reinterpret_cast<uint8_t *>(&schema_version), sizeof(uint16_t));
  if (!is_little_endian_) {
    swap_bytes(schema_version);
  }

  return schema_version;
}

void TcpServer::discard_connection() {
  client_.reset(nullptr);
}

uint32_t TcpServer::read_msg(uint8_t *buffer, uint32_t max_size) {
  uint32_t bytes_cnt;
  read_bytes(reinterpret_cast<uint8_t *>(&bytes_cnt), sizeof(uint32_t));
  if (!is_little_endian_) {
    swap_bytes(bytes_cnt);
  }

  if (bytes_cnt > max_size) {
    throw std::runtime_error(
        str_format("Can not read from socket: buffers max size less than msg (%u)", bytes_cnt));
  }
  read_bytes(buffer, bytes_cnt);
  return bytes_cnt;
}

void TcpServer::send_msg(uint8_t *buffer, uint32_t bytes_cnt) {
  if (!client_) {
    throw std::runtime_error("Can't send bytes if client is not connected.");
  }
  if (!client_->IsSocketValid()) {
    throw std::runtime_error(
        str_format("Can not send message, invalid socket: %s", client_->DescribeError()));
  }
  //  if (bytes_cnt == 0) {
  //    throw std::runtime_error("Can't send 0 bytes.");
  //  }

  uint32_t size_buffer = bytes_cnt;
  if (!is_little_endian_) {
    swap_bytes(size_buffer);
  }

  if (client_->Send(reinterpret_cast<uint8_t *>(&size_buffer), sizeof(uint32_t)) == 0) {
    throw std::runtime_error(
        str_format("Error sending message size: %s", client_->DescribeError()));
  }

  if (bytes_cnt > 0 && client_->Send(buffer, bytes_cnt) == 0) {
    throw std::runtime_error(str_format("Error sending message: %s", client_->DescribeError()));
  }
}

uint16_t TcpServer::get_port() const {
  return port_;
}

}  // namespace rewind_viewer::net

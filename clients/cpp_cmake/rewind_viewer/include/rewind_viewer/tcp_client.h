#pragma once

#include <clsocket/ActiveSocket.h>

#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>

#include "utils.h"

namespace rewind_viewer {

class TcpClient {
 private:
  std::string address_;
  uint16_t port_;
  std::unique_ptr<CActiveSocket> socket_;
  bool is_little_endian_;

  void read_bytes(uint8_t *buffer, uint32_t size) {
    if (!socket_) {
      throw std::runtime_error("Can't read bytes if socket is not connected.");
    }
    if (!socket_->IsSocketValid()) {
      throw std::runtime_error(
          str_format("Can't read bytes, invalid socket: %s", socket_->DescribeError()));
    }
    int pos = 0;
    while (size > 0) {
      int received = socket_->Receive(static_cast<int32_t>(size), buffer + pos);
      if (received > 0) {
        pos += received;
        size -= received;
      } else {
        auto error = str_format("Can't read bytes: %s", socket_->DescribeError());
        socket_.reset(nullptr);
        throw std::runtime_error(error);
      }
    }
  }

 public:
  TcpClient(std::string address, uint16_t port)
      : address_(std::move(address))
      , port_(port)
      , socket_(std::make_unique<CActiveSocket>(CActiveSocket::SocketTypeTcp)) {
    const int32_t value{0x01};
    const void *pointer{static_cast<const void *>(&value)};
    const unsigned char *least_significant_address{static_cast<const unsigned char *>(pointer)};
    is_little_endian_ = *least_significant_address == 0x01;
  }

  void connect(uint16_t schema_version) {
    socket_->DisableNagleAlgoritm();
    socket_->Initialize();
    if (!socket_->Open(address_.c_str(), port_)) {
      throw std::runtime_error(
          str_format("Can't connect to the server: %s", socket_->DescribeError()));
    }

    if (!is_little_endian_) {
      swap_bytes(schema_version);
    }

    if (socket_->Send(reinterpret_cast<uint8_t *>(&schema_version), sizeof(uint16_t)) == 0) {
      throw std::runtime_error(
          str_format("Error sending schema version: %s", socket_->DescribeError()));
    }
  }

  void disconnect() {
    if (socket_ && socket_->IsSocketValid()) {
      if (!socket_->Shutdown(CSimpleSocket::CShutdownMode::Both)) {
        // ???
      }
      socket_->Close();
    }
    socket_.reset(nullptr);
  }

  uint32_t read_msg(uint8_t *buffer, uint32_t max_size) {
    uint32_t bytes_cnt;
    read_bytes(reinterpret_cast<uint8_t *>(&bytes_cnt), sizeof(uint32_t));
    if (!is_little_endian_) {
      swap_bytes(bytes_cnt);
    }

    if (bytes_cnt > max_size) {
      throw std::runtime_error(str_format(
          "Cannot read from socket: buffer's max size less than message (%u)", bytes_cnt));
    }
    read_bytes(buffer, bytes_cnt);
    return bytes_cnt;
  }

  void send_msg(const uint8_t *buffer, uint32_t bytes_cnt) {
    if (!socket_) {
      throw std::runtime_error("Can't send bytes if socket is not connected.");
    }
    if (!socket_->IsSocketValid()) {
      throw std::runtime_error(
          str_format("Cannot send message, invalid socket: %s", socket_->DescribeError()));
    }

    uint32_t size_buffer = bytes_cnt;
    if (!is_little_endian_) {
      swap_bytes(size_buffer);
    }

    if (socket_->Send(reinterpret_cast<uint8_t *>(&size_buffer), sizeof(uint32_t)) == 0) {
      throw std::runtime_error(
          str_format("Error sending message size: %s", socket_->DescribeError()));
    }

    if (bytes_cnt > 0 && socket_->Send(buffer, bytes_cnt) == 0) {
      throw std::runtime_error(str_format("Error sending message: %s", socket_->DescribeError()));
    }
  }

  uint16_t get_port() const {
    return port_;
  }
};

}  // namespace rewind_viewer

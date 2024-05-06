#pragma once

#include <clsocket/PassiveSocket.h>

#include <bit>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace rewind_viewer::net {

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 5;

class TcpServer {
 private:
  std::string address_;
  uint16_t port_;
  std::unique_ptr<CPassiveSocket> socket_;
  std::unique_ptr<CActiveSocket> client_;
  bool is_little_endian_;

  void read_bytes(uint8_t *buffer, uint16_t size);

 public:
  TcpServer(std::string address, uint16_t port);
  void initialize();
  bool accept_connection();
  uint16_t read_msg(uint8_t *buffer, uint16_t max_size);
  void send_msg(uint8_t *buffer, uint16_t bytes_cnt);
  uint16_t get_port() const;
};

}  // namespace rewind_viewer::net

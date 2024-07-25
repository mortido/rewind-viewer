#pragma once

#include <clsocket/PassiveSocket.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace rewind_viewer::net {

class TcpServer {
 private:
  std::string address_;
  uint16_t port_;
  std::unique_ptr<CPassiveSocket> socket_;
  std::unique_ptr<CActiveSocket> client_;
  bool is_little_endian_;

  void read_bytes(uint8_t *buffer, uint32_t size);

 public:
  TcpServer(std::string address, uint16_t port);
  void initialize();
  uint16_t accept_connection();
  void discard_connection();
  uint32_t read_msg(uint8_t *buffer, uint32_t max_size);
  void send_msg(const uint8_t *buffer, uint32_t bytes_cnt);
  uint16_t get_port() const;
};

}  // namespace rewind_viewer::net

#pragma once

#include <clsocket/PassiveSocket.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "common/utils.h"
#include "gateway/transport/transport.h"

namespace rewind_viewer::gateway {

class TcpServer : public Transport {
 private:
  std::string address_;
  uint16_t port_;
  std::unique_ptr<CPassiveSocket> socket_;
  std::unique_ptr<CActiveSocket> client_;

 protected:
  void read_bytes(uint8_t *buffer, uint32_t size) override;
  void send_bytes(const uint8_t *buffer, uint32_t size) override;

 public:
  TcpServer(std::string address, uint16_t port);
  void initialize();
  bool accept_connection(int timeout = -1);
  void discard_connection();

  [[nodiscard]] uint16_t get_port() const {
    return port_;
  }
  [[nodiscard]] const std::string &get_address_() const {
    return address_;
  }
};

}  // namespace rewind_viewer::gateway

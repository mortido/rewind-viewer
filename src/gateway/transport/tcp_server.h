#pragma once

#include <clsocket/PassiveSocket.h>

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "common/utils.h"
#include "gateway/transport/transport.h"

namespace rewind_viewer::gateway {

class TcpServer : public ReusableTransport {
 private:
  std::string address_;
  uint16_t port_;
  std::string name_;
  std::unique_ptr<CPassiveSocket> socket_;
  std::unique_ptr<CActiveSocket> client_;

 protected:
  void read_bytes(uint8_t *buffer, uint32_t size) override;
  void send_bytes(const uint8_t *buffer, uint32_t size) override;

 public:
  TcpServer(std::string address, uint16_t port);

  bool accept_connection(int timeout) override;
  void close_connection() override;
  [[nodiscard]] bool is_connected() const override;

  [[nodiscard]] const std::string &get_name() const override {
    return name_;
  }
};

}  // namespace rewind_viewer::gateway

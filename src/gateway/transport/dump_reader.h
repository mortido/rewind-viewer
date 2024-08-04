#pragma once

#include <fstream>
#include <filesystem>
#include <string>

#include "gateway/transport/transport.h"

namespace rewind_viewer::gateway {

class DumpReader : public Transport {
 private:
  std::ifstream file_stream_;
  const std::string name_;
 protected:
  void read_bytes(uint8_t *buffer, uint32_t size) override;
  void send_bytes(const uint8_t *buffer, uint32_t size) override;

 public:
  DumpReader(const std::filesystem::path& file_path);
  void close_connection() override;
  [[nodiscard]] bool is_connected() const override;
  [[nodiscard]] const std::string &get_name() const override {
    return name_;
  }
};

}  // namespace rewind_viewer::gateway

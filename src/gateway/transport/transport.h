#pragma once
#include "common/utils.h"

namespace rewind_viewer::gateway {

class Transport {
 public:
  virtual ~Transport() = default;
  uint32_t read_msg(uint8_t *buffer, uint32_t max_size) {
    uint32_t bytes_cnt;
    read_bytes(reinterpret_cast<uint8_t *>(&bytes_cnt), sizeof(uint32_t));
    if (std::endian::native != std::endian::little) {
      swap_bytes(bytes_cnt);
    }

    if (bytes_cnt > max_size) {
      throw std::runtime_error(str_format(
          "Can not read from transport: buffers max size less than msg (%u)", bytes_cnt));
    }
    read_bytes(buffer, bytes_cnt);
    return bytes_cnt;
  }

  void send_msg(const uint8_t *buffer, uint32_t bytes_cnt) {
    uint32_t size_buffer = bytes_cnt;
    if (std::endian::native != std::endian::little) {
      swap_bytes(size_buffer);
    }

    send_bytes(reinterpret_cast<uint8_t *>(&size_buffer), sizeof(uint32_t));
    if (bytes_cnt > 0) {
      send_bytes(buffer, bytes_cnt);
    }
  }

  template <typename T>
  T read() {
    T value;
    read_bytes(reinterpret_cast<uint8_t *>(&value), sizeof(uint16_t));
    if (std::endian::native != std::endian::little) {
      swap_bytes(value);
    }
    return value;
  }

 protected:
  virtual void read_bytes(uint8_t *buffer, uint32_t size) = 0;
  virtual void send_bytes(const uint8_t *buffer, uint32_t size) = 0;
};
}  // namespace rewind_viewer::gateway
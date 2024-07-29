#pragma once

#include <memory>
#include <stdexcept>
#include <string>


template <typename... Args>
std::string str_format(const char *format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format, args...) + 1;
  if (size_s <= 0) {
    throw std::runtime_error("Error during string formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format, args...);
  return {buf.get(), buf.get() + size - 1};
}

template <typename T>
void swap_bytes(T &value) {
  auto *bytes = reinterpret_cast<uint8_t *>(&value);
  std::reverse(bytes, bytes + sizeof(T));
}

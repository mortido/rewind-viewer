#include "gateway/transport/dump_reader.h"

#include "common/logger.h"

namespace rewind_viewer::gateway {

void DumpReader::read_bytes(uint8_t *buffer, uint32_t size) {
  if (!file_stream_.is_open()) {
    throw std::runtime_error("Can't read bytes, file is not open.");
  }

  file_stream_.read(reinterpret_cast<char *>(buffer), size);
  if (!file_stream_) {
    throw std::runtime_error("Error reading bytes from file.");
  }
}

void DumpReader::send_bytes(const uint8_t * /*buffer*/, uint32_t /*size*/) {
  //  throw std::runtime_error("DumpReader does not support sending bytes.");
}

DumpReader::DumpReader(const std::filesystem::path &file_path) : name_(file_path.stem()) {
  file_stream_.open(file_path, std::ios::binary);
  if (!file_stream_) {
    throw std::runtime_error(str_format("Can't open file: %s", file_path.c_str()));
  }
  LOG_INFO("Opened file %s", file_path.c_str());
}

bool DumpReader::is_connected() const {
  return file_stream_.good();
}

void DumpReader::close_connection() {
  if (file_stream_.is_open()) {
    file_stream_.close();
  }
}

}  // namespace rewind_viewer::gateway

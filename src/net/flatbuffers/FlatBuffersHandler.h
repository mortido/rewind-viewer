#include <cstdint>

#include "net/MessageHandler.h"

namespace rewind_viewer::net {

struct ParsingError : std::runtime_error {
  using std::runtime_error::runtime_error;
};

class FlatBuffersHandler : public MessageHandler {
 public:
  using MessageHandler::MessageHandler;

  void handle_message(const uint8_t *data, int32_t nbytes) override;
};

}  // namespace rewind_viewer::net
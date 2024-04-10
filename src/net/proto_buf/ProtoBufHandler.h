#include <net/MessageHandler.h>

#include <cstdint>

namespace rewind_viewer::net {
class ProtoBufHandler : public MessageHandler {
 public:
    using MessageHandler::MessageHandler;

    void handle_message(const uint8_t *data, int32_t nbytes) override;
};

}  // namespace rewind_viewer::net
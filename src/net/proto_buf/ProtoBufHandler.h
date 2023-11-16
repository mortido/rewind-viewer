#include <net/ProtoHandler.h>

#include <cstdint>

namespace net {
class ProtoBufHandler : public ProtoHandler {
 public:
    using ProtoHandler::ProtoHandler;

    void handle_message(const uint8_t *data, int32_t nbytes) override;
};

}  // namespace net
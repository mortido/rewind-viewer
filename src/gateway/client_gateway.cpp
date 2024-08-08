#include "gateway/client_gateway.h"

#include "common/logger.h"

namespace rewind_viewer::gateway {

ClientGateway::ClientGateway(std::shared_ptr<models::Scene> scene,
                             std::shared_ptr<ReusableTransport> transport, bool master)
    : scene_editor_(std::move(scene), master)
    , state_{State::wait}
    , name_{transport->get_name()}
    , default_transport_{std::move(transport)} {
  transport_thread_ = std::thread(&ClientGateway::transport_loop, this);
  transport_thread_.detach();

  if (!default_transport_) {
    throw std::runtime_error("Default transport can't be nullptr");
  }
}

ClientGateway::~ClientGateway() {
  state_ = State::closed;
  if (transport_thread_.joinable()) {
    transport_thread_.join();
  }
}

std::unique_ptr<MessageHandler> ClientGateway::create_message_handler(std::shared_ptr<Transport> transport){
  try {
    auto schema_version = transport->read<uint16_t>();
    if (schema_version == JSON_MESSAGE_SCHEMA_VERSION) {
      return std::make_unique<JsonMessageHandler>(scene_editor_, transport);
    } else if(schema_version == FBS_MESSAGE_SCHEMA_VERSION) {
      return std::make_unique<FlatbuffersMessageHandler>(scene_editor_, transport);
    } else {
      LOG_ERROR("Unsupported schema version %u, update client code", schema_version);
    }
  } catch (std::exception const& e) {
    LOG_ERROR("Error reading schema version: %s", e.what());
  } catch (...) {
    LOG_ERROR("Unknown error reading schema version");
  }
  return nullptr;
}

std::unique_ptr<MessageHandler> ClientGateway::accept_connection() {
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    if (default_transport_->accept_connection(RETRY_TIMEOUT_MS)) {
      std::lock_guard lock(mutex_);
      name_ = default_transport_->get_name();
      scene_editor_.reset();
      return create_message_handler(default_transport_);
    } else {
      std::lock_guard lock(mutex_);
      if (one_use_transport_) {
        name_ = one_use_transport_->get_name();
        scene_editor_.reset();
        return create_message_handler(std::move(one_use_transport_));
      }
    }
  }
  return nullptr;
}

void ClientGateway::transport_loop() {
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    // It is ok to crash thread on accept connection error
    LOG_INFO("Waiting new connection...");
    auto message_handler = accept_connection();
    if (!message_handler) {
      // Closed
      break;
    }

    try {
      State expected = State::wait;
      if (state_.compare_exchange_strong(expected, State::established)) {
        while (state_.load(std::memory_order_relaxed) == State::established) {
          if (!message_handler->next_message(events_, actions_)) {
            break;
          }
        }
      }
    } catch (std::exception const& e) {
      LOG_ERROR("Error reading transport: %s", e.what());
    } catch (...) {
      LOG_ERROR("Unknown error during transport reading");
    }

    {
      std::lock_guard lock(mutex_);
      name_ = default_transport_->get_name();
    }
    events_.clear();
    actions_.clear();
    state_ = State::wait;
  }
}

}  // namespace rewind_viewer::gateway
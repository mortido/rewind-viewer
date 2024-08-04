#include "gateway/client_gateway.h"

#include "common/logger.h"

namespace rewind_viewer::gateway {

ClientGateway::ClientGateway(std::shared_ptr<models::Scene> scene,
                             std::shared_ptr<ReusableTransport> transport, bool master)
    : frame_editor_(std::move(scene), master)
    , read_buffer_(MAX_MESSAGE_SIZE)
    , state_{State::wait}
    , name_{transport->get_name()}
    , default_transport_{std::move(transport)} {
  message_handlers_[JSON_MESSAGE_SCHEMA_VERSION] = std::make_unique<JsonMessageHandler>();
  message_handlers_[FBS_MESSAGE_SCHEMA_VERSION] = std::make_unique<FlatbuffersMessageHandler>();
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

void ClientGateway::transport_loop() {
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    // It is ok to crash thread on accept connection error
    LOG_INFO("Waiting new connection...");
    while (!active_transport_ && state_.load(std::memory_order_relaxed) != State::closed) {
      if (default_transport_->accept_connection(RETRY_TIMEOUT_MS)) {
        std::lock_guard lock(mutex_);
        active_transport_ = default_transport_;
        name_ = active_transport_->get_name();
      } else {
        std::lock_guard lock(mutex_);
        if (one_use_transport_) {
          if (one_use_transport_->is_connected()) {
            active_transport_ = std::move(one_use_transport_);
            name_ = active_transport_->get_name();
          } else {
            one_use_transport_.reset();
            LOG_ERROR("One use transport is not connected: %s",
                      one_use_transport_->get_name().c_str());
          }
        }
      }
    }

    if (!active_transport_) {
      // Closed
      break;
    }

    try {
      auto schema_version = active_transport_->read<uint16_t>();
      if (message_handlers_.count(schema_version) > 0) {
        LOG_INFO("Schema version %u", schema_version);
        auto& message_handler = message_handlers_.at(schema_version);
        State expected = State::wait;
        if (state_.compare_exchange_strong(expected, State::established)) {
          frame_editor_.reset();
          while (state_.load(std::memory_order_relaxed) == State::established) {
            if (!active_transport_->is_connected()) {
              break;
            }
            uint32_t bytes = active_transport_->read_msg(read_buffer_.data(), read_buffer_.size());
            message_handler->handle_message(read_buffer_.data(), bytes, events_, actions_,
                                            frame_editor_, *active_transport_);
          }
        }
      } else {
        LOG_ERROR("Unsupported schema version %u, update client code", schema_version);
      }
    } catch (std::exception const& e) {
      LOG_ERROR("Error reading transport: %s", e.what());
    } catch (...) {
      LOG_ERROR("Unknown error during transport reading");
    }

    active_transport_->close_connection();
    active_transport_.reset();
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
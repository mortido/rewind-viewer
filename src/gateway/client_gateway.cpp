#include "gateway/client_gateway.h"

#include <rapidjson/error/en.h>

#include "common/logger.h"

namespace rewind_viewer::gateway {

ClientGateway::ClientGateway(std::shared_ptr<models::Scene> scene, const std::string& address,
                             uint16_t port, bool master)
    : frame_editor_(std::move(scene), master)
    , tcp_server_(address, port)
    , read_buffer_(MAX_MESSAGE_SIZE)
    , state_{State::wait} {
  message_handlers_[JSON_MESSAGE_SCHEMA_VERSION] = std::make_unique<JsonMessageHandler>();
  message_handlers_[FBS_MESSAGE_SCHEMA_VERSION] = std::make_unique<FlatbuffersMessageHandler>();
  network_thread_ = std::thread(&ClientGateway::network_loop, this);
  network_thread_.detach();
}

ClientGateway::~ClientGateway() {
  state_ = State::closed;
  if (network_thread_.joinable()) {
    network_thread_.join();
  }
}

void ClientGateway::stop() {
  state_ = State::closed;
}

void ClientGateway::network_loop() {
  // It is ok to crash thread on init error
  tcp_server_.initialize();
  while (state_.load(std::memory_order_relaxed) != State::closed) {
    // It is ok to crash thread on accept connection error
    LOG_INFO("Waiting new connection...");
    events_.clear();

    uint16_t schema_version_ = 0;
    while (schema_version_ == 0) {
      schema_version_ = tcp_server_.accept_connection();
      if (schema_version_ == 0) {
        // TODO: handle app exit better
        return;
      }
      if (message_handlers_.count(schema_version_) == 0) {
        LOG_ERROR("Unsupported schema version %u, update client code", schema_version_);
        tcp_server_.discard_connection();
        schema_version_ = 0;
      }
    }

    LOG_INFO("Schema version %u", schema_version_);
    auto& message_handler = message_handlers_.at(schema_version_);

    State expected = State::wait;
    if (state_.compare_exchange_strong(expected, State::established)) {
      frame_editor_.reset();
      while (state_.load(std::memory_order_relaxed) == State::established) {
        try {
          uint32_t bytes = tcp_server_.read_msg(read_buffer_.data(), read_buffer_.size());
          message_handler->handle_message(read_buffer_.data(), bytes, events_, frame_editor_,
                                          tcp_server_);
        } catch (std::exception const& e) {
          LOG_ERROR("Error reading message: %s", e.what());
          state_ = State::wait;
        } catch (...) {
          LOG_ERROR("Unknown error reading message");
          state_ = State::wait;
        }
      }
    }
  }
}

}  // namespace rewind_viewer::gateway
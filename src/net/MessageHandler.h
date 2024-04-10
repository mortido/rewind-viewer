#pragma once

#include "viewer/FrameEditor.h"
#include "viewer/Scene.h"

//namespace rewind_viewer::net {

class MessageHandler {
 public:
    enum class Mode {
        BATCH,     /// Wait to 'end' primitive before sending data
        IMMEDIATE  /// Send primitives as soon as they come
    };

    explicit MessageHandler(Scene *scene);
    virtual ~MessageHandler() = default;

    /// Called whenever data from socket should be processed
    /// data should be copied if wanted to be used after function call
    virtual void handle_message(const uint8_t *data, int32_t nbytes) = 0;

    /// Any saved data from old messages should be cleared on this call
    virtual void on_new_connection();

    void set_immediate_mode(bool enabled);

 protected:
    /// Should be called by specific handler after each processed message
    /// @param end_frame - set when 'end_frame' received
    void on_message_processed(bool end_frame);

    /// Get frame editor for normal or permanent frame
    /// Specific handler should use it to create primitives
    FrameEditor &get_frame_editor();

    /// Whenever to use normal or permanent frame for drawing
    void use_permanent_frame(bool use);

    /// Set primitives layer, affect both permanent and normal frames
    void set_layer(size_t layer);

    /// Set map config for scene
    void set_map_config(glm::vec2 size, glm::u16vec2 grid);

 private:
    void reset_state();

    Scene *scene_;
    std::shared_ptr<FrameEditor> frame_;
    FrameEditor permanent_frame_;
    bool use_permanent_ = false;

    Mode send_mode_{Mode::BATCH};
    // Data was transferred to last frame in immediate mode
    bool immediate_data_sent_{false};
    size_t last_layer_id_ = Frame::DEFAULT_LAYER;
};

//}  // namespace rewind_viewer::net
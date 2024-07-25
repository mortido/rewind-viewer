#pragma once
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

#include "clsocket/ActiveSocket.h"
#include "messages/rewind_event.fbs.h"
#include "messages/rewind_message.fbs.h"

namespace rewind_viewer {

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 5;

template <typename Vec2T>
struct RewindEvent {
  char key;
  std::vector<std::vector<Vec2T>> mouse_paths;
};

class RewindClient {
 public:
  RewindClient(const RewindClient &) = delete;
  RewindClient &operator=(const RewindClient &) = delete;

  RewindClient(const std::string &host, uint16_t port) : read_buffer_(MAX_MESSAGE_SIZE) {
    socket_.Initialize();
    socket_.DisableNagleAlgoritm();
    if (!socket_.Open(host.c_str(), port)) {
      fprintf(stderr, "RewindClient:: Cannot open viewer socket. Launch viewer before behavior\n");
    }

    // Could be std::endian::native == std::endian::little in c++20
    const int32_t value{0x01};
    const void *address{static_cast<const void *>(&value)};
    const unsigned char *least_significant_address{static_cast<const unsigned char *>(address)};
    is_little_endian_ = *least_significant_address == 0x01;

    // Send protocol version 1 time on connection.
    send_protocol_version();
  }

  ~RewindClient() {
    close();
  }

  void set_opacity(uint32_t opacity) {
    opacity_ = (opacity << 24u);
  }

  void end_frame() {
    builder_.Clear();
    auto command = fbs::CreateEndFrame(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_EndFrame, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void set_layer(size_t layer, bool permanent = false) {
    builder_.Clear();
    auto layer_obj = fbs::CreateLayer(builder_, layer, permanent);
    auto command = fbs::CreateOptions(builder_, 0, layer_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Options, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void map(const Vec2T &position, const Vec2T &size, uint16_t grid_x, uint16_t grid_y) {
    builder_.Clear();
    auto position_obj =
        fbs::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
    auto size_obj = fbs::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y));
    auto map_obj = fbs::CreateMap(builder_, &position_obj, &size_obj, grid_x, grid_y);
    auto command = fbs::CreateOptions(builder_, map_obj, 0);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Options, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void circle(const Vec2T &center, double r, uint32_t color, bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateCircle(builder_, color_obj, &center_obj, static_cast<float>(r));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Circle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void arc(const Vec2T &center, double r, double start_angle, double end_angle, uint32_t color,
           bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateArc(builder_, color_obj, &center_obj, static_cast<float>(r),
                                  static_cast<float>(start_angle), static_cast<float>(end_angle));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Arc, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_arc(const Vec2T &center, double r, double start_angle, double end_angle) {
    builder_.Clear();
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateArc(builder_, 0, &center_obj, static_cast<float>(r),
                                  static_cast<float>(start_angle), static_cast<float>(end_angle));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Arc, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void circle_segment(const Vec2T &center, double r, double start_angle, double end_angle,
                      uint32_t color, bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command =
        fbs::CreateCircleSegment(builder_, color_obj, &center_obj, static_cast<float>(r),
                                 static_cast<float>(start_angle), static_cast<float>(end_angle));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_CircleSegment, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_circle_segment(const Vec2T &center, double r, double start_angle, double end_angle) {
    builder_.Clear();
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command =
        fbs::CreateCircleSegment(builder_, 0, &center_obj, static_cast<float>(r),
                                 static_cast<float>(start_angle), static_cast<float>(end_angle));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_CircleSegment, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_circle(const Vec2T &center, double r) {
    builder_.Clear();
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateCircle(builder_, 0, &center_obj, static_cast<float>(r));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Circle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void line(const Vec2T &p1, const Vec2T &p2, uint32_t color) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, false);
    std::vector<fbs::Vector2f> points_obj;
    points_obj.emplace_back(static_cast<float>(p1.x), static_cast<float>(p1.y));
    points_obj.emplace_back(static_cast<float>(p2.x), static_cast<float>(p2.y));
    auto command =
        fbs::CreatePolyline(builder_, color_obj, builder_.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Polyline, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void polyline(const std::vector<Vec2T> &points, uint32_t color, bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    std::vector<fbs::Vector2f> points_obj;
    for (const auto &p : points) {
      points_obj.emplace_back(static_cast<float>(p.x), static_cast<float>(p.y));
    }
    auto command =
        fbs::CreatePolyline(builder_, color_obj, builder_.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Polyline, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_polyline(const std::vector<Vec2T> &points) {
    builder_.Clear();
    std::vector<fbs::Vector2f> points_obj;
    for (const auto &p : points) {
      points_obj.emplace_back(static_cast<float>(p.x), static_cast<float>(p.y));
    }
    auto command = fbs::CreatePolyline(builder_, 0, builder_.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Polyline, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  /*
   * Position specifies bottom left corner of the first cell.
   * Colors should be stored line by line.
   * Example:
   * [c1, c2, c3, c4] and elements_per_row = 2 wil represent the following field
   * | c3 | c4 |
   * | c1 | c2 |
   * With c1 - color on start position
   * elements_per_row = 3:
   * | c4 |
   * | c1 | c2 | c3 |
   */
  template <typename Vec2T>
  void tiles(const Vec2T &pos, const Vec2T &cell_size, uint16_t row_size,
             std::vector<uint32_t> *colors, bool use_global_alpha = true) {
    if (use_global_alpha) {
      for (auto &color : *colors) {
        color &= 0xFFFFFF;
        color |= opacity_;
      }
    }
    builder_.Clear();
    auto position_obj = fbs::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y));
    auto size_obj = fbs::Vector2f(static_cast<float>(cell_size.x), static_cast<float>(cell_size.y));
    auto command = fbs::CreateTiles(builder_, &position_obj, &size_obj, row_size,
                                    builder_.CreateVector(*colors));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Tiles, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void triangle(const Vec2T &p1, const Vec2T &p2, const Vec2T &p3, uint32_t color,
                bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    std::vector<fbs::Vector2f> points_obj;
    points_obj.emplace_back(static_cast<float>(p1.x), static_cast<float>(p1.y));
    points_obj.emplace_back(static_cast<float>(p2.x), static_cast<float>(p2.y));
    points_obj.emplace_back(static_cast<float>(p3.x), static_cast<float>(p3.y));
    auto command =
        fbs::CreateTriangle(builder_, color_obj, builder_.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Triangle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_triangle(const Vec2T &p1, const Vec2T &p2, const Vec2T &p3) {
    builder_.Clear();
    std::vector<fbs::Vector2f> points_obj;
    points_obj.emplace_back(static_cast<float>(p1.x), static_cast<float>(p1.y));
    points_obj.emplace_back(static_cast<float>(p2.x), static_cast<float>(p2.y));
    points_obj.emplace_back(static_cast<float>(p3.x), static_cast<float>(p3.y));
    auto command = fbs::CreateTriangle(builder_, 0, builder_.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Triangle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void rectangle(const Vec2T &position, const Vec2T &size, uint32_t color, bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreateRectangle(builder_, color_obj, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Rectangle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void mask_rectangle(const Vec2T &position, const Vec2T &size) {
    builder_.Clear();
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreateRectangle(builder_, 0, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Rectangle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  /**
   * Pass arbitrary user message to be stored in frame
   * Message content displayed in separate window inside viewer
   * Can be used several times per frame
   * It can be used like printf, e.g.: message("This %s will be %s", "string", "formatted")
   */
  template <typename... Args>
  void log_text(const char *fmt, Args... args) {
    builder_.Clear();
    auto str = builder_.CreateString(format(fmt, args...));
    auto command = fbs::CreateLogText(builder_, str);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_LogText, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T, typename... Args>
  void popup_round(const Vec2T &pos, double r, const char *fmt, Args... args) {
    builder_.Clear();
    auto str = builder_.CreateString(format(fmt, args...));
    fbs::Vector2f center_obj{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    auto command = fbs::CreatePopupRound(builder_, str, &center_obj, r);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_PopupRound, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T, typename... Args>
  void popup(const Vec2T &position, const Vec2T &size, const char *fmt, Args... args) {
    builder_.Clear();
    auto str = builder_.CreateString(format(fmt, args...));
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreatePopup(builder_, str, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Popup, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void camera_view(const std::string &name, const Vec2T &pos, double r) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    fbs::Vector2f pos_obj{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    auto command = fbs::CreateCameraView(builder_, name_obj, &pos_obj, r);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_CameraView, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void subscribe(const std::string &name, char key, bool continuous, bool capture_mouse,
                 double min_position_change = -1.0) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    flatbuffers::Offset<fbs::Subscribe> command;
    if (min_position_change > 0.0) {
      command = fbs::CreateSubscribe(builder_, name_obj, key, continuous, capture_mouse,
                                     min_position_change);
    } else {
      command = fbs::CreateSubscribe(builder_, name_obj, key, continuous, capture_mouse);
    }
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Subscribe, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void unsubscribe(char key) {
    builder_.Clear();
    auto command = fbs::CreateUnsubscribe(builder_, key);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Unsubscribe, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  std::vector<RewindEvent<Vec2T>> read_events() {
    builder_.Clear();
    auto command = fbs::CreateReadEvents(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_ReadEvents, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());

    // Buffer to store the incoming message size and content
    read_msg(read_buffer_.data());

    // Parse the received message
    auto event_list = fbs::GetRewindEventList(read_buffer_.data());
    std::vector<RewindEvent<Vec2T>> events;

    for (auto fbs_event : *event_list->events()) {
      auto &event = events.emplace_back(RewindEvent<Vec2T>{.key = fbs_event->key()});
      if (fbs_event->mouse_path()) {
        for (auto fbs_path : *fbs_event->mouse_path()) {
          auto &path = event.mouse_paths.emplace_back();
          for (auto fbs_point : *fbs_path->points()) {
            auto &point = path.emplace_back();
            point.x = fbs_point->x();
            point.y = fbs_point->y();
          }
        }
      }
    }
    return events;
  }

  void close() {
    if (socket_.IsSocketValid()) {
      if (!socket_.Shutdown(CSimpleSocket::CShutdownMode::Both)) {
        // ???
      }
      socket_.Close();
    }
  }

 private:
  bool is_little_endian_;
  flatbuffers::FlatBufferBuilder builder_;
  std::vector<uint8_t> read_buffer_;
  CActiveSocket socket_;
  uint32_t opacity_{0xFF000000};
  constexpr static uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

  void send_protocol_version() {
    static uint8_t buffer[sizeof(int16_t)];
    if (!is_little_endian_) {
      std::reverse(buffer, buffer + sizeof(uint16_t));
    }
    memcpy(buffer, &MESSAGE_SCHEMA_VERSION, sizeof(int16_t));
    socket_.Send(buffer, sizeof(int16_t));
  }

  void send(const uint8_t *buf, uint64_t buf_size) {
    //    if (buf_size > std::numeric_limits<uint32_t>::max()) {
    if (buf_size > MAX_MESSAGE_SIZE) {
      throw std::runtime_error("Rewind message size can't be more then 1MB");
    }
    static uint8_t buffer[sizeof(int32_t)];
    memcpy(buffer, &buf_size, sizeof(int32_t));
    if (!is_little_endian_) {
      std::reverse(buffer, buffer + sizeof(uint32_t));
    }
    socket_.Send(buffer, sizeof(int32_t));
    socket_.Send(buf, buf_size);
  }

  void read_bytes(uint8_t *buffer, uint32_t size) {
    if (!socket_.IsSocketValid()) {
      throw std::runtime_error("Can't read bytes if socket is not valid.");
    }
    int pos = 0;
    while (size > 0) {
      int received = socket_.Receive(static_cast<int32_t>(size), buffer + pos);
      if (received > 0) {
        pos += received;
        size -= received;
      } else {
        auto error = format("Can't read bytes: %s", socket_.DescribeError());
        socket_.Close();
        throw std::runtime_error(error);
      }
    }
  }

  uint32_t read_msg(uint8_t *buffer) {
    uint32_t bytes_cnt;
    read_bytes(reinterpret_cast<uint8_t *>(&bytes_cnt), sizeof(uint32_t));
    if (!is_little_endian_) {
      std::reverse(reinterpret_cast<uint8_t *>(&bytes_cnt),
                   reinterpret_cast<uint8_t *>(&bytes_cnt) + sizeof(uint32_t));
    }

    if (bytes_cnt > MAX_MESSAGE_SIZE) {
      throw std::runtime_error("Cannot read from socket: buffer's max size less than message size");
    }
    read_bytes(buffer, bytes_cnt);
    return bytes_cnt;
  }

  template <typename... Args>
  static inline std::string format(const char *fmt, Args... args) {
    thread_local char buf[2048];
    int bytes = snprintf(buf, sizeof(buf), fmt, args...);
    if (bytes < 0) {
      throw std::runtime_error("Encoding error in snprintf");
    }
    if (bytes >= sizeof(buf)) {
      throw std::runtime_error("Buffer size is too small for formatted string");
    }
    return {buf};
  }
};

}  // namespace rewind_viewer

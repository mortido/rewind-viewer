#pragma once

#include <vector>
#include <clsocket/ActiveSocket.h>

#include "messages/rewind_message.fbs.h"

namespace rewind_viewer {

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 4;

template<typename Vec2T>
class RewindClient {
 public:
  RewindClient(const RewindClient &) = delete;
  RewindClient &operator=(const RewindClient &) = delete;

  RewindClient(const std::string &host, uint16_t port) {
    socket_.Initialize();
    socket_.DisableNagleAlgoritm();
    if (!socket_.Open(host.c_str(), port)) {
      fprintf(stderr, "RewindClient:: Cannot open viewer socket. Launch viewer before behavior\n");
    }

    // Send protocol version 1 timee on connection.
    static uint8_t buffer[sizeof(int16_t)];
    // TODO: endianness
    memcpy(buffer, &MESSAGE_SCHEMA_VERSION, sizeof(int16_t));
    socket_.Send(buffer, sizeof(int16_t));
  }

  void set_opacity(uint32_t opacity) {
    opacity_ = (opacity << 24u);
  }

  void end_frame() {
    flatbuffers::FlatBufferBuilder builder;
    auto command = fbs::CreateEndFrame(builder);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_EndFrame, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void switch_to_layer(size_t layer, bool permanent = false) {
    flatbuffers::FlatBufferBuilder builder;
    auto layer_obj = fbs::CreateLayer(builder, layer, permanent);
    auto command = fbs::CreateOptions(builder, 0, layer_obj);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Options, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void map(const Vec2T &size, uint32_t grid_x, uint32_t grid_y) {
    flatbuffers::FlatBufferBuilder builder;
    auto map_obj = fbs::CreateMap(builder, size.x, size.y, grid_x, grid_y);
    auto command = fbs::CreateOptions(builder, map_obj, 0);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Options, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void circle(const Vec2T &center, double r, uint32_t color, bool fill = false) {
    flatbuffers::FlatBufferBuilder builder;
    auto color_obj = fbs::CreateColor(builder, color | opacity_, fill);
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateCircle(builder, color_obj, &center_obj, static_cast<float>(r));
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Circle, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void line(const Vec2T &p1, const Vec2T &p2, uint32_t color) {
    flatbuffers::FlatBufferBuilder builder;
    auto color_obj = fbs::CreateColor(builder, color | opacity_, false);
    std::vector<fbs::Vector2f> points_obj;
    points_obj.emplace_back(static_cast<float>(p1.x), static_cast<float>(p1.y));
    points_obj.emplace_back(static_cast<float>(p2.x), static_cast<float>(p2.y));
    auto command = fbs::CreatePolyline(builder, color_obj, builder.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Polyline, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void polyline(const std::vector<Vec2T> &points, uint32_t color) {
    flatbuffers::FlatBufferBuilder builder;
    auto color_obj = fbs::CreateColor(builder, color | opacity_, false);
    std::vector<fbs::Vector2f> points_obj;
    for (const auto &p : points) {
      points_obj.emplace_back(static_cast<float>(p.x), static_cast<float>(p.y));
    }
    auto command = fbs::CreatePolyline(builder, color_obj, builder.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Polyline, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void triangle(const Vec2T &p1, const Vec2T &p2, const Vec2T &p3, uint32_t color, bool fill = false) {
    flatbuffers::FlatBufferBuilder builder;
    auto color_obj = fbs::CreateColor(builder, color | opacity_, fill);
    std::vector<fbs::Vector2f> points_obj;
    points_obj.emplace_back(static_cast<float>(p1.x), static_cast<float>(p1.y));
    points_obj.emplace_back(static_cast<float>(p2.x), static_cast<float>(p2.y));
    points_obj.emplace_back(static_cast<float>(p3.x), static_cast<float>(p3.y));
    auto command = fbs::CreateTriangle(builder, color_obj, builder.CreateVectorOfStructs(points_obj));
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Triangle, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  void rectangle(const Vec2T &position, const Vec2T &size, uint32_t color, bool fill = false) {
    flatbuffers::FlatBufferBuilder builder;
    auto color_obj = fbs::CreateColor(builder, color | opacity_, fill);
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreateRectangle(builder, color_obj, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Rectangle, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  /**
   * Pass arbitrary user message to be stored in frame
   * Message content displayed in separate window inside viewer
   * Can be used several times per frame
   * It can be used like printf, e.g.: message("This %s will be %s", "string", "formatted")
   */
  template<typename... Args>
  void log_text(const char *fmt, Args... args) {
    flatbuffers::FlatBufferBuilder builder;
    auto str = builder.CreateString(format(fmt, args...));
    auto command = fbs::CreateLogText(builder, str);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_LogText, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  template<typename... Args>
  void popup_round(const Vec2T &pos, double r, const char *fmt, Args... args) {
    flatbuffers::FlatBufferBuilder builder;
    auto str = builder.CreateString(format(fmt, args...));
    fbs::Vector2f center_obj{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    auto command = fbs::CreatePopupRound(builder, str, &center_obj, r);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_PopupRound, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

  template<typename... Args>
  void popup(const Vec2T &position, const Vec2T &size, const char *fmt, Args... args) {
    flatbuffers::FlatBufferBuilder builder;
    auto str = builder.CreateString(format(fmt, args...));
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreatePopup(builder, str, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder, fbs::Command_Popup, command.Union());
    builder.Finish(msg);
    send(builder.GetBufferPointer(), builder.GetSize());
  }

 private:
  CActiveSocket socket_;
  uint32_t opacity_{0x0};

  void send(const uint8_t *buf, uint16_t buf_size) {
    static uint8_t buffer[sizeof(int16_t)];
    // TODO: endianness
    memcpy(buffer, &buf_size, sizeof(int16_t));
    socket_.Send(buffer, sizeof(int16_t));

    socket_.Send(buf, buf_size);
  }

  template<typename... Args>
  static inline std::string format(const char *fmt, Args... args) {
    static char buf[2048];
    int bytes = sprintf(buf, fmt, args...);
    buf[bytes] = '\0';
    return {buf};
  }
};

}  // namespace rewind_viewer

#pragma once
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "messages/events.fbs.h"
#include "messages/rewind_message.fbs.h"
#include "responses.h"
#include "tcp_client.h"
#include "utils.h"

namespace rewind_viewer {

constexpr uint16_t MESSAGE_SCHEMA_VERSION = 7;

enum class LayerOrigin {
  game,
  left_top,
  left_center,
  left_bottom,
  right_top,
  right_center,
  right_bottom,
  top_center,
  bottom_center
};

constexpr fbs::LayerOrigin to_fbs(LayerOrigin origin) {
  switch (origin) {
    case LayerOrigin::game: return fbs::LayerOrigin_GAME;
    case LayerOrigin::left_top: return fbs::LayerOrigin_LEFT_TOP;
    case LayerOrigin::left_center: return fbs::LayerOrigin_LEFT_CENTER;
    case LayerOrigin::left_bottom: return fbs::LayerOrigin_LEFT_BOTTOM;
    case LayerOrigin::right_top: return fbs::LayerOrigin_RIGHT_TOP;
    case LayerOrigin::right_center: return fbs::LayerOrigin_RIGHT_CENTER;
    case LayerOrigin::right_bottom: return fbs::LayerOrigin_RIGHT_BOTTOM;
    case LayerOrigin::top_center: return fbs::LayerOrigin_TOP_CENTER;
    case LayerOrigin::bottom_center: return fbs::LayerOrigin_BOTTOM_CENTER;
    default: return fbs::LayerOrigin_GAME;  // Default case, should never happen
  }
}

class RewindClient {
 private:
  constexpr static uint64_t MAX_MESSAGE_SIZE = 1024 * 1024;  // 1MB

  flatbuffers::FlatBufferBuilder builder_;
  std::vector<uint8_t> read_buffer_;
  uint32_t opacity_{0xFF000000};
  std::unique_ptr<rewind_viewer::TcpClient> tcp_client_;
  std::ofstream file_;
  size_t proto_id = 0;

  void send(const uint8_t *buf, uint64_t buf_size) {
    if (buf_size > MAX_MESSAGE_SIZE) {
      throw std::runtime_error("Rewind message size can't be more than 1MB");
    }
    if (tcp_client_) {
      tcp_client_->send_msg(buf, static_cast<uint32_t>(buf_size));

    } else {
      uint32_t size_buffer = buf_size;

      const int32_t value{0x01};
      const void *pointer{static_cast<const void *>(&value)};
      const unsigned char *least_significant_address{static_cast<const unsigned char *>(pointer)};
      bool is_little_endian = *least_significant_address == 0x01;
      if (!is_little_endian) {
        swap_bytes(size_buffer);
      }

      file_.write(reinterpret_cast<const char *>(&size_buffer), sizeof(size_buffer));
      file_.write(reinterpret_cast<const char *>(buf), static_cast<long>(buf_size));
    }
  }

  void send_action(const std::string &name, fbs::ActionInput input,
                   ::flatbuffers::Offset<void> input_offset) {
    auto name_obj = builder_.CreateString(name);
    auto command = fbs::CreateCreateAction(builder_, name_obj, input, input_offset);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_CreateAction, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

 public:
  RewindClient(const RewindClient &) = delete;
  RewindClient &operator=(const RewindClient &) = delete;

  RewindClient(const std::string &host, uint16_t port)
      : tcp_client_(std::make_unique<rewind_viewer::TcpClient>(host, port))
      , read_buffer_(MAX_MESSAGE_SIZE) {
    tcp_client_->connect(MESSAGE_SCHEMA_VERSION);
  }

  RewindClient(const std::filesystem::path &filename) : read_buffer_(MAX_MESSAGE_SIZE) {
    if (filename.empty()) {
      throw std::runtime_error("File path is empty");
    }
    auto parent_path = filename.parent_path();
    if (!parent_path.empty()) {
      std::filesystem::create_directories(filename.parent_path());
    }
    file_.open(filename, std::ios::binary | std::ios::trunc);
    if (!file_.is_open()) {
      throw std::runtime_error("Failed to open file: " + filename.string());
    }

    const int32_t value{0x01};
    const void *pointer{static_cast<const void *>(&value)};
    const unsigned char *least_significant_address{static_cast<const unsigned char *>(pointer)};
    bool is_little_endian = *least_significant_address == 0x01;

    uint16_t schema_version = MESSAGE_SCHEMA_VERSION;
    if (!is_little_endian) {
      swap_bytes(schema_version);
    }

    file_.write(reinterpret_cast<const char *>(&schema_version), sizeof(schema_version));
  }

  ~RewindClient() {
    if (tcp_client_) {
      tcp_client_->disconnect();
    }
  }

  void set_opacity(uint32_t opacity) { opacity_ = (opacity << 24u); }

  void start_proto() {
    builder_.Clear();
    auto command = fbs::CreateStartProto(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_StartProto, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  size_t end_proto() {
    builder_.Clear();
    auto command = fbs::CreateEndProto(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_EndProto, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
    return proto_id++;
  }

  template <typename Vec2T>
  void draw_proto(size_t id, const Vec2T &position, float angle = 0.0, uint32_t color = 0,
                  float scale = 1.0) {
    builder_.Clear();
    auto position_obj =
        fbs::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
    auto command = fbs::CreateDrawProto(builder_, id, &position_obj, angle, color, scale);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_DrawProto, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void end_frame() {
    builder_.Clear();
    auto command = fbs::CreateEndFrame(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_EndFrame, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void set_layer(size_t layer, bool permanent = false, LayerOrigin origin = LayerOrigin::game) {
    builder_.Clear();
    auto command = fbs::CreateLayer(builder_, layer, 0, permanent, to_fbs(origin));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Layer, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void set_layer_name(size_t layer, const std::string &name, bool permanent = false) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    auto command = fbs::CreateLayer(builder_, layer, name_obj, permanent);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Layer, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void map(const Vec2T &position, const Vec2T &size, uint16_t grid_x, uint16_t grid_y) {
    builder_.Clear();
    auto position_obj =
        fbs::Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
    auto size_obj = fbs::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y));
    auto command = fbs::CreateMap(builder_, &position_obj, &size_obj, grid_x, grid_y);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Map, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void circle(const Vec2T &center, float r, uint32_t color, bool fill = false) {
    builder_.Clear();
    auto color_obj = fbs::CreateColor(builder_, color | opacity_, fill);
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateCircle(builder_, color_obj, &center_obj, static_cast<float>(r));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Circle, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void arc(const Vec2T &center, float r, float start_angle, float end_angle, uint32_t color,
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
  void mask_arc(const Vec2T &center, float r, float start_angle, float end_angle) {
    builder_.Clear();
    auto center_obj = fbs::Vector2f(static_cast<float>(center.x), static_cast<float>(center.y));
    auto command = fbs::CreateArc(builder_, 0, &center_obj, static_cast<float>(r),
                                  static_cast<float>(start_angle), static_cast<float>(end_angle));
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Arc, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void circle_segment(const Vec2T &center, float r, float start_angle, float end_angle,
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
  void mask_circle_segment(const Vec2T &center, float r, float start_angle, float end_angle) {
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
  void mask_circle(const Vec2T &center, float r) {
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
    auto str = builder_.CreateString(str_format(fmt, args...));
    auto command = fbs::CreateLogText(builder_, str);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_LogText, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T, typename... Args>
  void popup_round(const Vec2T &pos, float r, const char *fmt, Args... args) {
    builder_.Clear();
    auto str = builder_.CreateString(str_format(fmt, args...));
    fbs::Vector2f center_obj{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    auto command = fbs::CreatePopupRound(builder_, str, &center_obj, r);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_PopupRound, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T, typename... Args>
  void popup(const Vec2T &position, const Vec2T &size, const char *fmt, Args... args) {
    builder_.Clear();
    auto str = builder_.CreateString(str_format(fmt, args...));
    fbs::Vector2f position_obj{static_cast<float>(position.x), static_cast<float>(position.y)};
    fbs::Vector2f size_obj{static_cast<float>(size.x), static_cast<float>(size.y)};
    auto command = fbs::CreatePopup(builder_, str, &position_obj, &size_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_Popup, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  template <typename Vec2T>
  void camera_view(const std::string &name, const Vec2T &pos, float r) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    fbs::Vector2f pos_obj{static_cast<float>(pos.x), static_cast<float>(pos.y)};
    auto command = fbs::CreateCameraView(builder_, name_obj, &pos_obj, r);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_CameraView, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }

  void subscribe(char key, const std::string &name, bool continuous, bool capture_mouse) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    flatbuffers::Offset<fbs::Subscribe> command;
    command = fbs::CreateSubscribe(builder_, key, name_obj, continuous, capture_mouse);
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
  EventsResponse<Vec2T> read_events() {
    if (file_.is_open()) {
      return {};  // Return an empty event list if using file
    }

    builder_.Clear();
    auto command = fbs::CreateReadEvents(builder_);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_ReadEvents, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());

    // Buffer to store the incoming message size and content
    tcp_client_->read_msg(read_buffer_.data(), MAX_MESSAGE_SIZE);

    // Parse the received message
    auto event_list = fbs::GetRewindEventList(read_buffer_.data());
    EventsResponse<Vec2T> result;

    for (auto fbs_event : *event_list->key_events()) {
      auto &event = result.events.emplace_back(RewindEvent<Vec2T>{.key = fbs_event->key()});
      if (fbs_event->mouse_paths()) {
        for (auto fbs_path : *fbs_event->mouse_paths()) {
          auto &path = event.mouse_paths.emplace_back();
          for (auto fbs_point : *fbs_path->points()) {
            auto &point = path.emplace_back();
            point.x = fbs_point->x();
            point.y = fbs_point->y();
          }
        }
      }
    }

    for (auto fbs_action : *event_list->action_events()) {
      ActionResult::Type type;
      ActionResult action(fbs_action->name()->str(), type);
      switch (fbs_action->value_type()) {
        case fbs::ActionValue::ActionValue_IntValue:
          type = ActionResult::Type::Int;
          action.set_value(fbs_action->value_as_IntValue()->value());
          break;
        case fbs::ActionValue::ActionValue_FloatValue:
          type = ActionResult::Type::Float;
          action.set_value(fbs_action->value_as_FloatValue()->value());
          break;
        case fbs::ActionValue::ActionValue_BoolValue:
          type = ActionResult::Type::Bool;
          action.set_value(fbs_action->value_as_BoolValue()->value());
          break;
        case fbs::ActionValue::ActionValue_StringValue:
          type = ActionResult::Type::String;
          action.set_value(fbs_action->value_as_StringValue()->value()->str());
          break;
        default: throw std::runtime_error("Unknown ActionValue type");
      }
      result.actions.push_back(std::move(action));
    }
    return result;
  }

  void create_button_action(const std::string &name) {
    builder_.Clear();
    auto input = fbs::CreateButtonInput(builder_);
    send_action(name, fbs::ActionInput::ActionInput_ButtonInput, input.Union());
  }

  void create_int_input_action(const std::string &name, int32_t default_value,
                               int32_t min_value = 0, int32_t max_value = 0) {
    builder_.Clear();
    auto input = fbs::CreateIntInput(builder_, default_value, min_value, max_value);
    send_action(name, fbs::ActionInput::ActionInput_IntInput, input.Union());
  }

  void create_float_input_action(const std::string &name, float default_value,
                                 float min_value = 0.0f, float max_value = 0.0f) {
    builder_.Clear();
    auto input = fbs::CreateFloatInput(builder_, default_value, min_value, max_value);
    send_action(name, fbs::ActionInput::ActionInput_FloatInput, input.Union());
  }

  void create_select_input_action(const std::string &name, const std::vector<std::string> &options,
                                  uint16_t selected_option) {
    builder_.Clear();
    std::vector<flatbuffers::Offset<flatbuffers::String>> option_offsets;
    for (const auto &option : options) {
      option_offsets.push_back(builder_.CreateString(option));
    }
    auto input =
        fbs::CreateSelectInput(builder_, builder_.CreateVector(option_offsets), selected_option);
    send_action(name, fbs::ActionInput::ActionInput_SelectInput, input.Union());
  }

  void create_string_input_action(const std::string &name, const std::string &default_value) {
    builder_.Clear();
    auto input = fbs::CreateStringInput(builder_, builder_.CreateString(default_value));
    send_action(name, fbs::ActionInput::ActionInput_StringInput, input.Union());
  }

  void create_bool_input_action(const std::string &name, bool default_value = false) {
    builder_.Clear();
    auto input = fbs::CreateBoolInput(builder_, default_value);
    send_action(name, fbs::ActionInput::ActionInput_BoolInput, input.Union());
  }

  void remove_action(const std::string &name) {
    builder_.Clear();
    auto name_obj = builder_.CreateString(name);
    auto command = fbs::CreateRemoveAction(builder_, name_obj);
    auto msg = fbs::CreateRewindMessage(builder_, fbs::Command_RemoveAction, command.Union());
    builder_.Finish(msg);
    send(builder_.GetBufferPointer(), builder_.GetSize());
  }
};

}  // namespace rewind_viewer

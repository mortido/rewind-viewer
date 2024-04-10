#include "ProtoBufHandler.h"

#include <common/logger.h>
#include <rewind_messages.pb.h>
#include <viewer/FrameEditor.h>

#include <cassert>
#include <string>

namespace {

void normalize(glm::vec2& min_corner, glm::vec2& max_corner) {
    if (min_corner.x > max_corner.x) {
        std::swap(min_corner.x, max_corner.x);
    }
    if (min_corner.y > max_corner.y) {
        std::swap(min_corner.y, max_corner.y);
    }
}

glm::vec4 convert_color(uint32_t value) {
    glm::vec4 result;
    result.r = static_cast<float>((value & 0xFF0000) >> 16) / 255.0f;
    result.g = static_cast<float>((value & 0x00FF00) >> 8) / 255.0f;
    result.b = static_cast<float>((value & 0x0000FF)) / 255.0f;

    uint8_t alpha = ((value & 0xFF000000) >> 24);
    if (alpha > 0) {
        result.a = static_cast<float>(alpha) / 255.0f;
    } else {
        result.a = 1.0f;
    }
    return result;
}

}  // anonymous namespace

struct ParsingError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

namespace rewind_viewer::net {
void ProtoBufHandler::handle_message(const uint8_t* data, int32_t nbytes) {
    proto::rewind::DrawMessage msg;
    msg.ParseFromArray(data, nbytes);
    auto& ctx = get_frame_editor().context();

    if (msg.has_circle()) {
        LOG_V8("ProtoBufHandler::CIRCLE");
        const auto& circle = msg.circle();
        ctx.add_circle({circle.center().x(), circle.center().y()}, circle.radius(),
                       convert_color(circle.color().value()), circle.color().fill());
    } else if (msg.has_triangle()) {
        LOG_V8("ProtoBufHandler::TRIANGLE");
        const auto& triangle = msg.triangle();
        // TODO: restore gradient colors.
        if (triangle.points_size() != 3) {
            throw ParsingError{"Triangle expect exactly 3 points, got " +
                               std::to_string(triangle.points_size())};
        }
        ctx.add_triangle({triangle.points(0).x(),triangle.points(0).y()},
                         {triangle.points(1).x(),triangle.points(1).y()},
                         {triangle.points(2).x(),triangle.points(2).y()},
                         convert_color(triangle.color().value()), triangle.color().fill());

    } else if (msg.has_polyline()) {
        LOG_V8("ProtoBufHandler::POLYLINE");
        const auto& polyline = msg.polyline();
        if (polyline.points_size() < 2) {
            throw ParsingError{"Polyline expect exactly 2 or more points, got " +
                               std::to_string(polyline.points_size())};
        }
        std::vector<glm::vec2> points;
        for (const auto& p:polyline.points()) {
            points.emplace_back(p.x(), p.y());
        }
        ctx.add_polyline(points, convert_color(polyline.color().value()));

    } else if (msg.has_rectangle()) {
        LOG_V8("ProtoBufHandler::RECTANGLE");
        const auto& rectangle = msg.rectangle();
        // TODO: restore gradient colors.
        glm::vec2 top_left{rectangle.top_left().x(),rectangle.top_left().y()};
        glm::vec2 bottom_right{rectangle.bottom_right().x(),rectangle.bottom_right().y()};
        normalize(top_left, bottom_right);
        ctx.add_rectangle(top_left, bottom_right,
                          convert_color(rectangle.color().value()), rectangle.color().fill());

    } else if (msg.has_popup()) {
        LOG_V8("ProtoBufHandler::POPUP");
        const auto& popup = msg.popup();
        get_frame_editor().add_box_popup({popup.position().x(), popup.position().y()},
                                         {popup.width(), popup.height()}, popup.text());
    } else if (msg.has_popup_round()) {
        LOG_V8("ProtoBufHandler::POPUP_ROUND");
        const auto& popup = msg.popup_round();
        get_frame_editor().add_round_popup({popup.center().x(), popup.center().y()}, popup.radius(),
                                           popup.text());
    } else if (msg.has_map()) {
        LOG_V8("ProtoBufHandler::MAP");
        const auto& map = msg.map();
        set_map_config({map.width(), map.height()}, {map.x_grid(), map.y_grid()});
    } else if (msg.has_options()) {
        LOG_V8("ProtoBufHandler::OPTIONS");
        const auto& options = msg.options();
        if (options.has_use_permanent_frame()) {
            use_permanent_frame(options.use_permanent_frame());
        }
        if (options.has_layer()) {
            set_layer(options.layer());
        }
    } else if (msg.has_log_text()) {
        LOG_V8("ProtoBufHandler::LOG_TEXT");
        get_frame_editor().add_user_text(msg.log_text().text());
    } else if (msg.has_end_frame()) {
        LOG_V8("ProtoBufHandler::END_FRAME");

    } else {
        LOG_ERROR("Unknown primitve type");
    }

    on_message_processed(msg.has_end_frame());
}

}  // namespace rewind_viewer::net
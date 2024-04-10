#include "FlatBuffersHandler.h"

#include <common/logger.h>
#include <rewind_messages.fbs.h>
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

namespace rewind_viewer::net {

void FlatBuffersHandler::handle_message(const uint8_t* data, [[maybe_unused]] int32_t nbytes) {
  auto msg = fbs::GetRewindMessage(data);
  auto& ctx = get_frame_editor().context();

  switch (msg->command_type()) {
    case fbs::Command_Circle: {
      LOG_V8("FlatBuffersHandler::CIRCLE");
      auto circle = msg->command_as_Circle();
      if (circle->radius() <= 0.0) {
        throw ParsingError{"Circle radius should be positive, got " +
                           std::to_string(circle->radius())};
      }
      ctx.add_circle({circle->center()->x(), circle->center()->y()}, circle->radius(),
                     convert_color(circle->color()->value()), circle->color()->fill());
      break;
    }
    case fbs::Command_Triangle: {
      LOG_V8("FlatBuffersHandler::TRIANGLE");
      auto triangle = msg->command_as_Triangle();
      if (triangle->points()->size() != 3) {
        throw ParsingError{"Triangle expect exactly 3 points, got " +
                           std::to_string(triangle->points()->size())};
      }
      std::vector<glm::vec2> points;
      for (auto p : *triangle->points()) {
        points.emplace_back(p->x(), p->y());
      }
      ctx.add_triangle(points[0], points[1], points[2], convert_color(triangle->color()->value()),
                       triangle->color()->fill());
      break;
    }
    case fbs::Command_Polyline: {
      LOG_V8("FlatBuffersHandler::POLYLINE");
      auto polyline = msg->command_as_Polyline();
      if (polyline->points()->size() < 2) {
        throw ParsingError{"Polyline expect exactly 2 or more points, got " +
                           std::to_string(polyline->points()->size())};
      }
      std::vector<glm::vec2> points;
      for (auto p : *polyline->points()) {
        points.emplace_back(p->x(), p->y());
      }
      ctx.add_polyline(points, convert_color(polyline->color()->value()));
      break;
    }
    case fbs::Command_Rectangle: {
      LOG_V8("FlatBuffersHandler::RECTANGLE");
      auto rectangle = msg->command_as_Rectangle();
      if (rectangle->size()->x() <= 0.0) {
        throw ParsingError{"Rectangle width should be positive, got " +
                           std::to_string(rectangle->size()->x())};
      }
      if (rectangle->size()->y() <= 0.0) {
        throw ParsingError{"Rectangle height should be positive, got " +
                           std::to_string(rectangle->size()->y())};
      }
      // TODO: restore gradient colors feature.
      glm::vec2 top_left{rectangle->position()->x(), rectangle->position()->y()};
      glm::vec2 bottom_right{top_left.x + rectangle->size()->x(),
                             top_left.y + rectangle->size()->x()};
      normalize(top_left, bottom_right);
      ctx.add_rectangle(top_left, bottom_right, convert_color(rectangle->color()->value()),
                        rectangle->color()->fill());
      break;
    }
    case fbs::Command_Popup: {
      LOG_V8("FlatBuffersHandler::POPUP");
      auto popup = msg->command_as_Popup();
      if (popup->area_size()->x() <= 0.0) {
        throw ParsingError{"Popup area width should be positive, got " +
                           std::to_string(popup->area_size()->x())};
      }
      if (popup->area_size()->y() <= 0.0) {
        throw ParsingError{"Popup area height should be positive, got " +
                           std::to_string(popup->area_size()->y())};
      }
      get_frame_editor().add_box_popup({popup->area_position()->x(), popup->area_position()->y()},
                                       {popup->area_size()->x(), popup->area_size()->y()},
                                       popup->text()->str());
      break;
    }
    case fbs::Command_PopupRound: {
      LOG_V8("FlatBuffersHandler::POPUP_ROUND");
      auto popup = msg->command_as_PopupRound();
      if (popup->area_radius() <= 0.0) {
        throw ParsingError{"Popup area radius should be positive, got " +
                           std::to_string(popup->area_radius())};
      }
      get_frame_editor().add_round_popup({popup->area_center()->x(), popup->area_center()->y()},
                                         popup->area_radius(), popup->text()->str());
      break;
    }
    case fbs::Command_Options: {
      LOG_V8("FlatBuffersHandler::OPTIONS");
      auto options = msg->command_as_Options();
      if (options->map() != nullptr) {
        LOG_V8("FlatBuffersHandler::OPTIONS->MAP");
        auto map = options->map();
        if (map->width() <= 0.0) {
          throw ParsingError{"Map width should be positive, got " + std::to_string(map->width())};
        }
        if (map->height() <= 0.0) {
          throw ParsingError{"Map height should be positive, got " + std::to_string(map->height())};
        }
        set_map_config({map->width(), map->height()}, {map->x_grid(), map->y_grid()});
      }
      if (options->layer() != nullptr) {
        LOG_V8("FlatBuffersHandler::OPTIONS->LAYER");
        auto layer = options->layer();
        use_permanent_frame(layer->use_permanent_frame());
        set_layer(layer->id());
      }
      break;
    }
    case fbs::Command_LogText: {
      LOG_V8("FlatBuffersHandler::LOG_TEXT");
      auto logText = msg->command_as_LogText();
      get_frame_editor().add_user_text(logText->text()->str());
      break;
    }
    case fbs::Command_EndFrame: {
      LOG_V8("FlatBuffersHandler::END_FRAME");
      break;
    }
    default: {
      LOG_ERROR("Unknown command type");
      break;
    }
  }

  on_message_processed(msg->command_type() == fbs::Command_EndFrame);
}

}  // namespace rewind_viewer::net

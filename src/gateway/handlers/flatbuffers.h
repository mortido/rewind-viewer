#pragma once

#include <flatbuffers/flatbuffers.h>

#include "gateway/handlers/abstract.h"
#include "gateway/messages/rewind_event.fbs.h"
#include "gateway/messages/rewind_message.fbs.h"

namespace rewind_viewer::gateway {
class FlatbuffersMessageHandler : public MessageHandler {
 private:
  flatbuffers::FlatBufferBuilder builder_;

 public:
  void handle_message(const uint8_t* buffer, uint32_t, EventsCollection& events,
                      models::FrameEditor& frame_editor, Transport& transport) override {
    auto message = fbs::GetRewindMessage(buffer);
    switch (message->command_type()) {
      case fbs::Command_Circle: {
        LOG_V8("FlatBuffersHandler::CIRCLE");
        auto circle = message->command_as_Circle();
        if (circle->radius() <= 0.0) {
          throw ParsingError{"Circle radius should be positive, got " +
                             std::to_string(circle->radius())};
        }

        if (circle->color() == nullptr) {
          frame_editor.add_stencil_circle({circle->center()->x(), circle->center()->y()},
                                          circle->radius());
        } else {
          frame_editor.add_circle({circle->center()->x(), circle->center()->y()}, circle->radius(),
                                  circle->color()->value(), circle->color()->fill());
        }
        break;
      }
      case fbs::Command_CircleSegment: {
        LOG_V8("FlatBuffersHandler::CIRCLE_SEGMENT");
        auto segment = message->command_as_CircleSegment();
        if (segment->radius() <= 0.0) {
          throw ParsingError{"Circle segment radius should be positive, got " +
                             std::to_string(segment->radius())};
        }
        if (segment->color() == nullptr) {
          frame_editor.add_stencil_segment({segment->center()->x(), segment->center()->y()},
                                           segment->radius(), segment->start_angle(),
                                           segment->end_angle());
        } else {
          frame_editor.add_segment({segment->center()->x(), segment->center()->y()},
                                   segment->radius(), segment->start_angle(), segment->end_angle(),
                                   segment->color()->value(), segment->color()->fill());
        }
        break;
      }
      case fbs::Command_Arc: {
        LOG_V8("FlatBuffersHandler::ARC");
        auto arc = message->command_as_Arc();
        if (arc->radius() <= 0.0) {
          throw ParsingError{"Arc radius should be positive, got " + std::to_string(arc->radius())};
        }
        if (arc->color() == nullptr) {
          frame_editor.add_stencil_arc({arc->center()->x(), arc->center()->y()}, arc->radius(),
                                       arc->start_angle(), arc->end_angle());
        } else {
          frame_editor.add_arc({arc->center()->x(), arc->center()->y()}, arc->radius(),
                               arc->start_angle(), arc->end_angle(), arc->color()->value(),
                               arc->color()->fill());
        }
        break;
      }
      case fbs::Command_Triangle: {
        LOG_V8("FlatBuffersHandler::TRIANGLE");
        auto triangle = message->command_as_Triangle();
        if (triangle->points()->size() != 3) {
          throw ParsingError{"Triangle expect exactly 3 points, got " +
                             std::to_string(triangle->points()->size())};
        }
        std::vector<glm::vec2> points;
        for (auto p : *triangle->points()) {
          points.emplace_back(p->x(), p->y());
        }
        if (triangle->color() == nullptr) {
          frame_editor.add_stencil_triangle(points[0], points[1], points[2]);
        } else {
          // TODO: support for gradient colors
          frame_editor.add_triangle(points[0], points[1], points[2], triangle->color()->value(),
                                    triangle->color()->fill());
        }
        break;
      }
      case fbs::Command_Polyline: {
        LOG_V8("FlatBuffersHandler::POLYLINE");
        auto polyline = message->command_as_Polyline();
        if (polyline->points()->size() < 2) {
          throw ParsingError{"Polyline expect exactly 2 or more points, got " +
                             std::to_string(polyline->points()->size())};
        }
        if (polyline->color() == nullptr) {
          throw ParsingError{"Polyline is not supported mask for now"};
        }
        if (polyline->color()->fill()) {
          LOG_ERROR("Polyline is not supporting fill for now. Parameter ignored.");
        }
        std::vector<glm::vec2> points;
        for (auto p : *polyline->points()) {
          points.emplace_back(p->x(), p->y());
        }
        frame_editor.add_polyline(points, polyline->color()->value());
        break;
      }
      case fbs::Command_Rectangle: {
        LOG_V8("FlatBuffersHandler::RECTANGLE");
        auto rectangle = message->command_as_Rectangle();
        if (rectangle->size()->x() <= 0.0) {
          throw ParsingError{"Rectangle width should be positive, got " +
                             std::to_string(rectangle->size()->x())};
        }
        if (rectangle->size()->y() <= 0.0) {
          throw ParsingError{"Rectangle height should be positive, got " +
                             std::to_string(rectangle->size()->y())};
        }
        glm::vec2 top_left{rectangle->position()->x(), rectangle->position()->y()};
        glm::vec2 bottom_right{top_left.x + rectangle->size()->x(),
                               top_left.y + rectangle->size()->y()};
        normalize_AABB(top_left, bottom_right);
        if (rectangle->color() == nullptr) {
          frame_editor.add_stencil_rectangle(top_left, bottom_right);
        } else {
          frame_editor.add_rectangle(top_left, bottom_right, rectangle->color()->value(),
                                     rectangle->color()->fill());
        }
        break;
      }
      case fbs::Command_Popup: {
        LOG_V8("FlatBuffersHandler::POPUP");
        auto popup = message->command_as_Popup();
        if (popup->area_size()->x() <= 0.0) {
          throw ParsingError{"Popup area width should be positive, got " +
                             std::to_string(popup->area_size()->x())};
        }
        if (popup->area_size()->y() <= 0.0) {
          throw ParsingError{"Popup area height should be positive, got " +
                             std::to_string(popup->area_size()->y())};
        }

        frame_editor.add_box_popup(

            {popup->area_position()->x() + 0.5 * popup->area_size()->x(),
             popup->area_position()->y() + 0.5 * popup->area_size()->y()},
            {popup->area_size()->x(), popup->area_size()->y()}, popup->text()->str());
        break;
      }
      case fbs::Command_PopupRound: {
        LOG_V8("FlatBuffersHandler::POPUP_ROUND");
        auto popup = message->command_as_PopupRound();
        if (popup->area_radius() <= 0.0) {
          throw ParsingError{"Popup area radius should be positive, got " +
                             std::to_string(popup->area_radius())};
        }

        frame_editor.add_round_popup({popup->area_center()->x(), popup->area_center()->y()},
                                     popup->area_radius(), popup->text()->str());
        break;
      }
      case fbs::Command_CameraView: {
        LOG_V8("FlatBuffersHandler::CAMERA_VIEW");
        auto cam_view_msg = message->command_as_CameraView();
        models::CameraView cam_view{
            {cam_view_msg->position()->x(), cam_view_msg->position()->y()},
            {cam_view_msg->view_radius() * 2.0f, cam_view_msg->view_radius() * 2.0f}};

        frame_editor.add_camera_view(cam_view_msg->name()->str(), cam_view);
        break;
      }
      case fbs::Command_Options: {
        LOG_V8("FlatBuffersHandler::OPTIONS");
        auto options = message->command_as_Options();
        if (options->map() != nullptr) {
          LOG_V8("FlatBuffersHandler::OPTIONS->MAP");

          auto map = options->map();
          if (map->size()->x() <= 0.0) {
            throw ParsingError{"Map width should be positive, got " +
                               std::to_string(map->size()->x())};
          }
          if (map->size()->y() <= 0.0) {
            throw ParsingError{"Map height should be positive, got " +
                               std::to_string(map->size()->y())};
          }
          frame_editor.set_map({map->position()->x(), map->position()->y()},
                               {map->size()->x(), map->size()->y()},
                               {map->x_grid(), map->y_grid()});
        }
        if (options->layer() != nullptr) {
          LOG_V8("FlatBuffersHandler::OPTIONS->LAYER");
          auto layer = options->layer();
          frame_editor.set_layer(layer->id(), layer->use_permanent_frame());
        }
        break;
      }
      case fbs::Command_LogText: {
        LOG_V8("FlatBuffersHandler::LOG_TEXT");
        auto logText = message->command_as_LogText();

        frame_editor.add_user_text(logText->text()->str());
        break;
      }
      case fbs::Command_EndFrame: {
        LOG_V8("FlatBuffersHandler::END_FRAME");
        frame_editor.finish_frame();
        break;
      }
      case fbs::Command_Tiles: {
        LOG_V8("FlatBuffersHandler::COLOR_FIELD");
        auto tiles = message->command_as_Tiles();
        glm::vec2 cell{tiles->cell_size()->x(), tiles->cell_size()->y()};
        glm::vec2 position{tiles->position()->x(), tiles->position()->y()};
        //      position -= 0.5f * cell;
        float start_x = position.x;
        auto row_size = tiles->row_size();
        if (row_size == 0) {
          throw ParsingError{"Tiles row size should be greater than 0"};
        }
        size_t i = 0;
        for (auto color : *tiles->colors()) {
          if (color & 0xFF000000) {  // Ignore transparent
            frame_editor.add_rectangle(position, position + cell, color, true);
          }
          if (++i == row_size) {
            i = 0;
            position.y += cell.y;
            position.x = start_x;
          } else {
            position.x += cell.x;
          }
        }
        break;
      }
      case fbs::Command_Subscribe: {
        LOG_V8("FlatBuffersHandler::SUBSCRIBE");
        auto subscribe = message->command_as_Subscribe();
        if (subscribe->capture_mouse()) {
          events.add(subscribe->key(), std::make_unique<CursorEvent>(
                                           subscribe->name()->str(), subscribe->continuous(),
                                           subscribe->key(), subscribe->min_position_change()));

        } else {
          events.add(subscribe->key(),
                     std::make_unique<KeyEvent>(subscribe->name()->str(), subscribe->continuous(),
                                                subscribe->key()));
        }
        break;
      }
      case fbs::Command_Unsubscribe: {
        LOG_V8("FlatBuffersHandler::UNSUBSCRIBE");
        auto unsubscribe = message->command_as_Unsubscribe();
        events.remove(unsubscribe->key());
        break;
      }
      case fbs::Command_ReadEvents: {
        LOG_V8("FlatBuffersHandler::READ_EVENTS");
        builder_.Clear();
        auto event_offsets = events.serialize(builder_);
        events.reset();
        auto events_vector = builder_.CreateVector(event_offsets);
        auto event_list = rewind_viewer::fbs::CreateRewindEventList(builder_, events_vector);
        builder_.Finish(event_list);
        transport.send_msg(builder_.GetBufferPointer(), builder_.GetSize());
        break;
      }
      default: {
        LOG_ERROR("Unknown command type");
        break;
      }
    }
  }
};
}  // namespace rewind_viewer::gateway
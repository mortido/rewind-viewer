#pragma once

#include <flatbuffers/flatbuffers.h>

#include "gateway/handlers/abstract.h"
#include "gateway/messages/events.fbs.h"
#include "gateway/messages/rewind_message.fbs.h"

namespace rewind_viewer::gateway {
class FlatbuffersMessageHandler : public MessageHandler {
 private:
  flatbuffers::FlatBufferBuilder builder_;

 public:
  FlatbuffersMessageHandler(models::SceneEditor& scene_editor, std::shared_ptr<Transport> transport)
      : MessageHandler(scene_editor, std::move(transport)) {}

  void handle_message(uint32_t /*size*/, LockDictionary<char, std::unique_ptr<Event>>& events,
                      LockDictionary<std::string, std::unique_ptr<Action>>& actions) override {
    auto message = fbs::GetRewindMessage(read_buffer_.data());
    switch (message->command_type()) {
      case fbs::Command_Circle: {
        LOG_V8("FlatBuffersHandler::CIRCLE");
        auto circle = message->command_as_Circle();
        if (circle->radius() <= 0.0) {
          throw ParsingError{"Circle radius should be positive, got " +
                             std::to_string(circle->radius())};
        }

        if (circle->color() == nullptr) {
          scene_editor_.add_stencil_circle(glm::vec2{circle->center()->x(), circle->center()->y()},
                                           circle->radius());
        } else {
          scene_editor_.add_circle(glm::vec2{circle->center()->x(), circle->center()->y()},
                                   circle->radius(), circle->color()->value(),
                                   circle->color()->fill());
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
          scene_editor_.add_stencil_segment(
              glm::vec2{segment->center()->x(), segment->center()->y()}, segment->radius(),
              segment->start_angle(), segment->end_angle());
        } else {
          scene_editor_.add_segment(glm::vec2{segment->center()->x(), segment->center()->y()},
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
          scene_editor_.add_stencil_arc(glm::vec2{arc->center()->x(), arc->center()->y()},
                                        arc->radius(), arc->start_angle(), arc->end_angle());
        } else {
          scene_editor_.add_arc(glm::vec2{arc->center()->x(), arc->center()->y()}, arc->radius(),
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
          scene_editor_.add_stencil_triangle(points[0], points[1], points[2]);
        } else {
          // TODO: support for gradient colors
          scene_editor_.add_triangle(points[0], points[1], points[2], triangle->color()->value(),
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
        scene_editor_.add_polyline(points, polyline->color()->value());
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
          scene_editor_.add_stencil_rectangle(top_left, bottom_right);
        } else {
          scene_editor_.add_rectangle(top_left, bottom_right, rectangle->color()->value(),
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

        scene_editor_.add_box_popup(

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

        scene_editor_.add_round_popup({popup->area_center()->x(), popup->area_center()->y()},
                                      popup->area_radius(), popup->text()->str());
        break;
      }
      case fbs::Command_CameraView: {
        LOG_V8("FlatBuffersHandler::CAMERA_VIEW");
        auto cam_view_msg = message->command_as_CameraView();
        models::CameraView cam_view{
            {cam_view_msg->position()->x(), cam_view_msg->position()->y()},
            {cam_view_msg->view_radius() * 2.0f, cam_view_msg->view_radius() * 2.0f}};

        scene_editor_.add_camera_view(cam_view_msg->name()->str(), cam_view);
        break;
      }
      case fbs::Command_Layer: {
        LOG_V8("FlatBuffersHandler::OPTIONS->LAYER");
        auto layer = message->command_as_Layer();
        scene_editor_.set_layer(layer->id(), layer->use_permanent_frame(),
                                static_cast<models::CameraOrigin>(layer->origin()));
        if (layer->name()) {
          scene_editor_.set_layer_name(layer->id(), layer->name()->str(),
                                       layer->use_permanent_frame());
        }
        break;
      }
      case fbs::Command_Map: {
        LOG_V8("FlatBuffersHandler::OPTIONS->MAP");
        auto map = message->command_as_Map();
        if (map->size()->x() <= 0.0) {
          throw ParsingError{"Map width should be positive, got " +
                             std::to_string(map->size()->x())};
        }
        if (map->size()->y() <= 0.0) {
          throw ParsingError{"Map height should be positive, got " +
                             std::to_string(map->size()->y())};
        }
        scene_editor_.set_map({map->position()->x(), map->position()->y()},
                              {map->size()->x(), map->size()->y()}, {map->x_grid(), map->y_grid()});
        break;
      }
      case fbs::Command_LogText: {
        LOG_V8("FlatBuffersHandler::LOG_TEXT");
        auto logText = message->command_as_LogText();

        scene_editor_.add_user_text(logText->text()->str());
        break;
      }
      case fbs::Command_EndFrame: {
        LOG_V8("FlatBuffersHandler::END_FRAME");
        scene_editor_.finish_frame();
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
            scene_editor_.add_rectangle(position, position + cell, color, true);
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
          events.add(subscribe->key(),
                     std::make_unique<CursorEvent>(subscribe->key(), subscribe->name()->str(),
                                                   subscribe->continuous()));

        } else {
          events.add(subscribe->key(),
                     std::make_unique<KeyEvent>(subscribe->key(), subscribe->name()->str(),
                                                subscribe->continuous()));
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
        std::vector<flatbuffers::Offset<rewind_viewer::fbs::KeyEvent>> event_offsets;
        std::vector<flatbuffers::Offset<rewind_viewer::fbs::ActionEvent>> actions_offsets;
        events.iterate([&](auto, auto& event) {
          if (event->is_triggered()) {
            event_offsets.push_back(event->serialize(builder_));
            event->reset_state();
          }
        });
        actions.iterate([&](auto, auto& action) {
          if (action->is_triggered()) {
            actions_offsets.push_back(action->serialize(builder_));
            action->reset_state();
          }
        });
        auto events_vector = builder_.CreateVector(event_offsets);
        auto actions_vector = builder_.CreateVector(actions_offsets);
        auto event_list =
            rewind_viewer::fbs::CreateRewindEventList(builder_, events_vector, actions_vector);
        builder_.Finish(event_list);
        transport_->send_msg(builder_.GetBufferPointer(), builder_.GetSize());
        break;
      }
      case fbs::Command_CreateAction: {
        LOG_V8("FlatBuffersHandler::CREATE_ACTION");
        auto create_action = message->command_as_CreateAction();
        const auto action_name = create_action->name()->str();

        switch (create_action->input_type()) {
          case fbs::ActionInput_BoolInput: {
            auto bool_input = create_action->input_as_BoolInput();
            actions.add(action_name, std::make_unique<BoolInputAction>(
                                         action_name, bool_input->default_value()));
            break;
          }
          case fbs::ActionInput_ButtonInput: {
            actions.add(action_name, std::make_unique<ButtonAction>(action_name));
            break;
          }
          case fbs::ActionInput_FloatInput: {
            auto float_input = create_action->input_as_FloatInput();
            actions.add(action_name, std::make_unique<FloatInputAction>(
                                         action_name, float_input->default_value(),
                                         float_input->min_value(), float_input->max_value()));
            break;
          }
          case fbs::ActionInput_IntInput: {
            auto int_input = create_action->input_as_IntInput();
            actions.add(action_name, std::make_unique<IntInputAction>(
                                         action_name, int_input->default_value(),
                                         int_input->min_value(), int_input->max_value()));
            break;
          }
          case fbs::ActionInput_SelectInput: {
            auto select_input = create_action->input_as_SelectInput();
            std::vector<std::string> options;
            for (const auto& option : *select_input->options()) {
              options.push_back(option->str());
            }
            actions.add(action_name,
                        std::make_unique<SelectInputAction>(action_name, std::move(options),
                                                            select_input->selected_option()));
            break;
          }
          case fbs::ActionInput_StringInput: {
            auto string_input = create_action->input_as_StringInput();
            actions.add(action_name, std::make_unique<StringInputAction>(
                                         action_name, string_input->default_value()->str()));
            break;
          }
          case fbs::ActionInput_NONE:
          default: {
            LOG_ERROR("Unknown or Empty ActionType for CreateAction");
          }
        }
        break;
      }
      case fbs::Command_RemoveAction: {
        LOG_V8("FlatBuffersHandler::REMOVE_ACTION");
        auto remove_action = message->command_as_RemoveAction();
        const auto& action_name = remove_action->name()->str();
        actions.remove(action_name);
        break;
      }
      case fbs::Command_StartProto: {
        LOG_V8("FlatBuffersHandler::START_PROTO");
        scene_editor_.start_proto();
        break;
      }
      case fbs::Command_EndProto: {
        LOG_V8("FlatBuffersHandler::END_PROTO");
        scene_editor_.end_proto();
        break;
      }
      case fbs::Command_DrawProto: {
        LOG_V8("FlatBuffersHandler::DRAW_PROTO");
        auto command = message->command_as_DrawProto();
        scene_editor_.add_proto(command->id(), {command->position()->x(), command->position()->y()},
                                command->angle(), command->color(), command->scale());
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
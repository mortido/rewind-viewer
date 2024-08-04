#include <chrono>
#include <random>
#include <thread>

#include "rewind_viewer/colors.h"
#include "rewind_viewer/plot.h"
#include "rewind_viewer/rewind_client.h"

using namespace rewind_viewer::colors;
using namespace std::chrono_literals;

// Your vector implementation, should have fields 'x', 'y'
struct Vec2D {
  double x;
  double y;
};

void draw_pattern(rewind_viewer::RewindClient &rc, const Vec2D &pos, int i) {
  rc.mask_circle(Vec2D{pos.x + 15.0, pos.y + 75.0}, 6.0);
  rc.mask_circle_segment(Vec2D{pos.x + 15.0, pos.y + 61.0}, 6.0, 2.443, 3.84);
  rc.mask_circle_segment(Vec2D{pos.x + 15.0, pos.y + 61.0}, 6.0, -1.745, 1.745);
  rc.mask_arc(Vec2D{pos.x + 15.0, pos.y + 47.0}, 6.0, 2.443, 3.84);
  rc.mask_arc(Vec2D{pos.x + 15.0, pos.y + 47.0}, 6.0, -1.745, 1.745);

  rc.mask_triangle(Vec2D{pos.x + 25.0, pos.y + 40.0}, Vec2D{pos.x + 35.0, pos.y + 20.0},
                   Vec2D{pos.x + 15.0, pos.y + 20.0});
  rc.mask_rectangle(Vec2D{pos.x + 35.0, pos.y + 45.0}, Vec2D{20.0, 20.0});

  rc.circle(Vec2D{pos.x + 25.0, pos.y + 50.0}, 20.0, purple::Magenta, true);
  rc.arc(Vec2D{pos.x + 50.0, pos.y + 12.0}, 6.0, 2.443, 3.84, purple::Magenta, true);
  rc.arc(Vec2D{pos.x + 50.0, pos.y + 12.0}, 6.0, -1.745, 1.745, purple::Magenta, true);
  rc.circle_segment(Vec2D{pos.x + 50.0, pos.y + 88.0}, 6.0, 2.443, 3.84, purple::Magenta, true);
  rc.circle_segment(Vec2D{pos.x + 50.0, pos.y + 88.0}, 6.0, -1.745, 1.745, purple::Magenta, true);

  rc.circle(Vec2D{pos.x + 75.0, pos.y + 50.0}, 20.0, purple::Magenta);
  rc.arc(Vec2D{pos.x + 75.0, pos.y + 50.0}, 17.5, 2.443, 3.84, purple::Magenta);
  rc.arc(Vec2D{pos.x + 75.0, pos.y + 50.0}, 17.5, -1.745, 1.745, purple::Magenta);
  rc.circle_segment(Vec2D{pos.x + 75.0, pos.y + 50.0}, 15.0, 2.443, 3.84, purple::Magenta);
  rc.circle_segment(Vec2D{pos.x + 75.0, pos.y + 50.0}, 15.0, -1.745, 1.745, purple::Magenta);

  // Not supported for permanent frames
  rc.popup_round(Vec2D{pos.x + 75.0, pos.y + 50.0}, 20.0, "Round popup #%d", i);

  rc.triangle(Vec2D{pos.x + 25.0, pos.y + 50.0}, Vec2D{pos.x + 45.0, pos.y + 15.0},
              Vec2D{pos.x + 5.0, pos.y + 15.0}, blue::Cyan, true);
  rc.triangle(Vec2D{pos.x + 75.0, pos.y + 50.0}, Vec2D{pos.x + 95.0, pos.y + 15.0},
              Vec2D{pos.x + 55.0, pos.y + 15.0}, blue::Cyan);
  rc.rectangle(Vec2D{pos.x + 10.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, yellow::Yellow, true);
  rc.rectangle(Vec2D{pos.x + 60.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, yellow::Yellow);

  // Not supported for permanent frames
  rc.popup(Vec2D{pos.x + 60.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, "Rectangular popup #%d", i);

  rc.line(Vec2D{pos.x + 5.0, pos.y + 5.0}, Vec2D{pos.x + 95.0, pos.y + 95.0}, green::Green);
  rc.polyline<Vec2D>({Vec2D{pos.x + 5.0, pos.y + 95.0}, Vec2D{pos.x + 40.0, pos.y + 80.0},
                      Vec2D{pos.x + 60.0, pos.y + 20.0}, Vec2D{pos.x + 95.0, pos.y + 5.0}},
                     red::Crimson);
}

void subscribe(rewind_viewer::RewindClient &rc) {
  rc.subscribe("draw circle", 'g', false, true);
  rc.subscribe("draw line", 'f', true, true);
  rc.subscribe("left", 'a', true, false);
  rc.subscribe("right", 'd', true, false);
  rc.subscribe("down", 's', true, false);
  rc.subscribe("up", 'w', true, false);

  rc.create_button_action("Test Button Action");
  rc.create_int_input_action("Test Int Input Action", 42, 10, 100);
  rc.create_int_input_action("Test Int Input Action 2", 42);
  rc.create_float_input_action("Test Float Input Action", 3.14f, 0.0f, 10.0f);
  rc.create_float_input_action("Test Float Input Action 2", 3.14f);
  rc.create_select_input_action("Test Select Input Action", {"Option 1", "Option 2", "Option 3"}, 0);
  rc.create_string_input_action("Test String Input Action 2", "Default Text");
  rc.create_bool_input_action("Test Bool Input Action", true);
  rc.create_bool_input_action("Test Bool Input Action 2", false);
}

int main(int argc, char *argv[]) {
  // Should be just 1 instance.
  // Pass it via reference, shared_ptr ot create singleton to access it.
  // Assume rewind viewer is started on the same host with default port.
  rewind_viewer::RewindClient rewind_client("127.0.0.1", 9111);
//  rewind_viewer::RewindClient rewind_client("example_output.rwn");
  subscribe(rewind_client);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, 23);

  Vec2D map_pos{0.0, 0.0};
  Vec2D map_size{500.0, 400.0};
  rewind_client.map(map_pos, map_size, 5, 40);
  std::vector<uint32_t> field_colors;

  Vec2D cell_size{5.0, 10.0};
  Vec2D start_pos{0.0, 0.0};
  Vec2D pos = start_pos;
  uint16_t row_size = static_cast<uint16_t>(map_size.x / cell_size.x);
  while (pos.y < map_size.y) {
    pos.x = start_pos.x;
    while (pos.x < map_size.x) {
      uint32_t color = 127;
      if (pos.x < map_size.x * 0.5 || distrib(gen) != 4) {
        color |= static_cast<uint32_t>(255.0 * (pos.y / map_size.y)) << 24;
      }
      color |= static_cast<uint32_t>(255.0 * (pos.y / map_size.y)) << 8;
      color |= static_cast<uint32_t>(255.0 * (pos.x / map_size.x)) << 16;
      field_colors.push_back(color);
      pos.x += cell_size.x;
    }
    pos.y += cell_size.y;
  }

  Vec2D pattern_position{};
  std::uniform_real_distribution<> dis_y(-50.0, 50.0);
  std::uniform_real_distribution<> dis_y_2(-5.0, 5.0);
  rewind_viewer::Plot<Vec2D> plot({250.0, -75.0}, {2.5, 1.0}, {0.0, 0.0});
  plot.set_marker_size(0.7);
  plot.set_axis_color(yellow::LightGoldenrodYellow);
  plot.add_series("value_1", blue::Aquamarine);
  plot.add_series("value_2", orange::OrangeRed);
  rewind_client.set_layer(9, true);
  plot.draw_axes_lines(rewind_client, Vec2D{-100.0, -50.0}, Vec2D{100.0, 50.0});

  double plot_x = -100.0;
  double value_1 = dis_y(gen);
  double value_2 = dis_y(gen);

  for (int i = 0; i < 10; i++) {
    rewind_client.log_text("Iteration: %d", i);
    rewind_client.log_text("Extra message");
    rewind_client.log_text("Multi\nline\nmessage");

    rewind_client.set_layer(0);
    rewind_client.tiles(start_pos, cell_size, row_size, &field_colors, false);

    // Normal layer
    rewind_client.set_layer(i);
    rewind_client.set_opacity(255 - i * 15);
    pattern_position.x = static_cast<int>(i % 5) * 100.0;
    pattern_position.y = static_cast<int>(i / 5) * 200.0;
    draw_pattern(rewind_client, pattern_position, i);

    rewind_client.camera_view("Allways Camera", pattern_position, 100.0);

    auto events = rewind_client.read_events<Vec2D>();
    while (events.empty()) {
      std::this_thread::sleep_for(1000ms);
      events = rewind_client.read_events<Vec2D>();
    }
    for (auto &event : events) {
      if (event.key == 'g') {
        rewind_client.circle(event.mouse_paths[0][0], 10, green::MediumAquamarine, true);
      } else if (event.key == 'f') {
        for(const auto& path: event.mouse_paths){
          if (path.empty()){
            //todo: throw?
            continue ;
          }

          for(size_t i =1; i < path.size();i++){
            rewind_client.line(path[i-1], path[i], orange::OrangeRed);
            rewind_client.circle(path[i], 1, green::MediumAquamarine, true);
          }

        }
      }
    }

    // Permanent layer
    rewind_client.set_layer(i, true);
    pattern_position.y += 100.0;
    draw_pattern(rewind_client, pattern_position, i);

    if (i % 2) {
      rewind_client.camera_view("Odd Camera", pattern_position, 100.0);
    }

    // Generate random data for the plot
    for (int x = 0; x < 20; ++x) {
      value_1 += dis_y_2(gen);
      value_2 += dis_y_2(gen);
      plot.add_value(0, plot_x, value_1);
      plot.add_value(1, plot_x, value_2);
      plot_x += 1.0;
    }

    rewind_client.set_opacity(0xff);
    rewind_client.set_layer(9, true);
    plot.draw_new(rewind_client, true);

    rewind_client.end_frame();
  }

//  rewind_client.set_layer(5);
//  Vec2D position{800.0,0.0};
//  for (int i = 0; i < 1000; i++) {
//
//    auto events = rewind_client.read_events<Vec2D>();
//    for (auto &event : events) {
//      if (event.key == 'a') {
//        position.x -= 5.0;
//      } else if (event.key == 'd') {
//        position.x += 5.0;
//      } else if (event.key == 's') {
//        position.y -= 5.0;
//      } else if (event.key == 'w') {
//        position.y += 5.0;
//      }
//    }
//
//    rewind_client.circle(position, 5, orange::OrangeRed, true);
//
//    std::this_thread::sleep_for(50ms);
//    rewind_client.end_frame();
//  }

  return EXIT_SUCCESS;
}

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
  if (i >= 0) {
    rc.popup_round(Vec2D{pos.x + 75.0, pos.y + 50.0}, 20.0, "Round popup #%d", i);
  }
  rc.triangle(Vec2D{pos.x + 25.0, pos.y + 50.0}, Vec2D{pos.x + 45.0, pos.y + 15.0},
              Vec2D{pos.x + 5.0, pos.y + 15.0}, blue::Cyan, true);
  rc.triangle(Vec2D{pos.x + 75.0, pos.y + 50.0}, Vec2D{pos.x + 95.0, pos.y + 15.0},
              Vec2D{pos.x + 55.0, pos.y + 15.0}, blue::Cyan);
  rc.rectangle(Vec2D{pos.x + 10.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, yellow::Yellow, true);
  rc.rectangle(Vec2D{pos.x + 60.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, yellow::Yellow);

  // Not supported for permanent frames
  if (i >= 0) {
    rc.popup(Vec2D{pos.x + 60.0, pos.y + 40.0}, Vec2D{20.0, 40.0}, "Rectangular popup #%d", i);
  }

  rc.line(Vec2D{pos.x + 5.0, pos.y + 5.0}, Vec2D{pos.x + 95.0, pos.y + 95.0}, green::Green);
  rc.polyline<Vec2D>({Vec2D{pos.x + 5.0, pos.y + 95.0}, Vec2D{pos.x + 40.0, pos.y + 80.0},
                      Vec2D{pos.x + 60.0, pos.y + 20.0}, Vec2D{pos.x + 95.0, pos.y + 5.0}},
                     red::Crimson);
}

void setup(rewind_viewer::RewindClient &rc) {
  rc.set_layer_name(0, "layer 1");
  rc.set_layer_name(1, "layer 2");
  rc.set_layer_name(2, "layer 3");
  rc.set_layer_name(3, "layer 4");
  rc.set_layer_name(4, "layer 5");
  rc.set_layer_name(5, "layer 6");
  rc.set_layer_name(9, "layer 10");
  rc.set_layer_name(9, "Perma layer 10", true);
}

void subscribe(rewind_viewer::RewindClient &rc) {
  rc.subscribe('g', "draw", false, true);
  rc.subscribe('h', "draw white", false, true);
  rc.subscribe('f', "draw line", true, true);
  rc.subscribe('a', "left", true, false);
  rc.subscribe('d', "right", true, false);
  rc.subscribe('s', "down", true, false);
  rc.subscribe('w', "up", true, false);

  //  rc.create_button_action("Button");
  //  rc.create_int_input_action("Int Action", 42, 10, 100);
  //  rc.create_int_input_action("Int Action 2", 42);
  rc.create_float_input_action("Scale", 1.0f, 0.1f, 5.0f);
  rc.create_float_input_action("Angle", 0.0f, 0.0f, 6.28f);
  //  rc.create_float_input_action("Float Action 2", 3.14f);
  //  rc.create_select_input_action("Select Action", {"Option 1", "Option 2", "Option 3"}, 0);
  //  rc.create_string_input_action("String Action", "Default Text");
  //  rc.create_bool_input_action("Bool Action", true);
  //  rc.create_bool_input_action("Bool Action 2", false);
}

void draw_HUD(rewind_viewer::RewindClient &rc) {
  Vec2D zero;
  Vec2D text_pos_1{-40.0,50.0};
  Vec2D text_pos_2{-40.0,0.0};

  rc.set_layer(0, true, rewind_viewer::LayerOrigin::left_top);
  rc.circle(zero, 50, red::Crimson, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(1, true, rewind_viewer::LayerOrigin::left_center);
  rc.circle(zero, 50, pink::HotPink, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(2, true, rewind_viewer::LayerOrigin::left_bottom);
  rc.circle(zero, 50, orange::Tomato, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(3, true, rewind_viewer::LayerOrigin::right_top);
  rc.circle(zero, 50, yellow::Gold, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(4, true, rewind_viewer::LayerOrigin::right_center);
  rc.circle(zero, 50, purple::Orchid, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(5, true, rewind_viewer::LayerOrigin::right_bottom);
  rc.circle(zero, 50, green::LimeGreen, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(6, true, rewind_viewer::LayerOrigin::top_center);
  rc.circle(zero, 50, blue::SkyBlue, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
  rc.set_layer(7, true, rewind_viewer::LayerOrigin::bottom_center);
  rc.circle(zero, 50, brown::Chocolate, true);
  rc.text(text_pos_1, 20.0, 0xFFFFFF00, "\U0001F434❤");
  rc.text(text_pos_2, 20.0, 0xFFFFFF00, "❤\U0001F984");
}

bool handle_events(rewind_viewer::RewindClient &rc, size_t pattern_id) {
  bool was_draw = false;
  static float scale = 1.0;
  static float angle = 0.0;

  auto response = rc.read_events<Vec2D>();
  for (auto &action : response.actions) {
    if (action.get_name() == "Scale") {
      scale = action.get_float_value();
    } else if (action.get_name() == "Angle") {
      angle = action.get_float_value();
    }
  }
  for (auto &event : response.events) {
    if (event.key == 'g') {
      rc.draw_proto(pattern_id, event.mouse_paths[0][0], angle, 0, scale);
      was_draw = true;
    } else if (event.key == 'h') {
      rc.draw_proto(pattern_id, event.mouse_paths[0][0], angle, 0x77FFFFFF, scale);
      was_draw = true;
    } else if (event.key == 'f') {
      for (const auto &path : event.mouse_paths) {
        for (size_t j = 1; j < path.size(); j++) {
          rc.line(path[j - 1], path[j], orange::OrangeRed);
//          rc.circle(path[j], 1, green::MediumAquamarine, true);
          rc.text(path[j], 10.0, 0xFFFFFF00, "\U0001F434❤");
        }
      }
      was_draw = true;
    }
  }

  return was_draw;
}

void draw_field(rewind_viewer::RewindClient &rc, Vec2D &map_size) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> distrib(0, 23);

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
  rc.tiles(start_pos, cell_size, row_size, &field_colors, false);
}

int main(int argc, char *argv[]) {
  // Should be just 1 instance.
  // Pass it via reference, shared_ptr ot create singleton to access it.
  // Assume rewind viewer is started on the same host with default port.
  rewind_viewer::RewindClient rc("127.0.0.1", 9111);
  //  rewind_viewer::RewindClient rewind_client("example_output.rwn");
  setup(rc);
  subscribe(rc);

  rc.start_proto();
  // Draw pattern once.
  draw_pattern(rc, Vec2D{-50.0, -50.0}, -1);
  size_t pattern_proto = rc.end_proto();

  Vec2D map_pos{0.0, 0.0};
  Vec2D map_size{500.0, 400.0};
  rc.map(map_pos, map_size, 5, 40);

  rc.set_layer(0, true);
  draw_field(rc, map_size);

  Vec2D pattern_position{};
  std::uniform_real_distribution<> dis_y(-50.0, 50.0);
  std::uniform_real_distribution<> dis_y_2(-5.0, 5.0);
  rewind_viewer::Plot<Vec2D> plot({70.0, 70.0}, {2.5, 1.0}, {0.0, 0.0});
  plot.set_marker_size(0.7);
  plot.set_axis_color(yellow::LightGoldenrodYellow);
  plot.add_series("value_1", blue::Aquamarine);
  plot.add_series("value_2", orange::OrangeRed);
  rc.set_layer(9, true, rewind_viewer::LayerOrigin::left_bottom);
  plot.draw_axes_lines(rc, Vec2D{-10.0, -50.0}, Vec2D{210.0, 50.0});

  draw_HUD(rc);

  double plot_x = 0.0;
  double value_1 = 0;
  double value_2 = 0;
  std::random_device rd;
  std::mt19937 gen(rd());

  for (int i = 0; i < 10; i++) {
    rc.log_text("Iteration: %d", i);
    rc.log_text("Extra message");
    rc.log_text("Multi\nline\nmessage");

    // Normal layer
    rc.set_layer(i);
    uint32_t opacity = 255 - i * 15;
    rc.set_opacity(255 - i * 15);
    pattern_position.x = static_cast<int>(i % 5) * 100.0;
    pattern_position.y = static_cast<int>(i / 5) * 200.0;
    draw_pattern(rc, pattern_position, i);

    rc.text(pattern_position, 10.0, 0xFFFF00 | (opacity << 24), "Hello, \U0001F434!");

    rc.camera_view("Allways Camera", pattern_position, 100.0);

    // Permanent layer
    rc.set_layer(i, true);
    pattern_position.y += 100.0;
    draw_pattern(rc, pattern_position, i);

    if (i % 2) {
      rc.camera_view("Odd Camera", pattern_position, 100.0);
    }

    // Generate random data for the plot
    for (int x = 0; x < 20; ++x) {
      value_1 += dis_y_2(gen);
      value_2 += dis_y_2(gen);
      plot.add_value(0, plot_x, value_1);
      plot.add_value(1, plot_x, value_2);
      plot_x += 1.0;
    }

    rc.set_opacity(0xff);
    rc.set_layer(9, true, rewind_viewer::LayerOrigin::left_bottom);
    plot.draw_new(rc, true);

    rc.end_frame();
  }

  rc.set_layer(5, true);
  float angle = 0.0;
  float scale = 1.0;
  for (int i = 0; i < 20; i++) {
    bool was_draw = false;
    while (!handle_events(rc, pattern_proto)) {
      std::this_thread::sleep_for(100ms);
    }
    rc.end_frame();
  }

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

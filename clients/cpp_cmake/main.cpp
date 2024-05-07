#include <random>

#include "rewind_viewer/RewindClient.h"
#include "rewind_viewer/colors.h"

// Your vector implementation, should have fields 'x', 'y'
struct Vec2D {
  double x;
  double y;
};

void draw_pattern(rewind_viewer::RewindClient<Vec2D> &rc, const Vec2D &pos, int i) {
  rc.mask_circle({pos.x + 15.0, pos.y + 75.0}, 6.0);
  rc.mask_circle_segment({pos.x + 15.0, pos.y + 61.0}, 6.0, 2.443, 3.84);
  rc.mask_circle_segment({pos.x + 15.0, pos.y + 61.0}, 6.0, -1.745, 1.745);
  rc.mask_arc({pos.x + 15.0, pos.y + 47.0}, 6.0, 2.443, 3.84);
  rc.mask_arc({pos.x + 15.0, pos.y + 47.0}, 6.0, -1.745, 1.745);

  rc.mask_triangle({pos.x + 25.0, pos.y + 40.0}, {pos.x + 35.0, pos.y + 20.0},
                   {pos.x + 15.0, pos.y + 20.0});
  rc.mask_rectangle({pos.x + 35.0, pos.y + 45.0}, {20.0, 20.0});

  rc.circle({pos.x + 25.0, pos.y + 50.0}, 20.0, rewind_viewer::colors::purple::Magenta, true);
  rc.arc({pos.x + 50.0, pos.y + 12.0}, 6.0, 2.443, 3.84, rewind_viewer::colors::purple::Magenta,
         true);
  rc.arc({pos.x + 50.0, pos.y + 12.0}, 6.0, -1.745, 1.745, rewind_viewer::colors::purple::Magenta,
         true);
  rc.circle_segment({pos.x + 50.0, pos.y + 88.0}, 6.0, 2.443, 3.84,
                    rewind_viewer::colors::purple::Magenta, true);
  rc.circle_segment({pos.x + 50.0, pos.y + 88.0}, 6.0, -1.745, 1.745,
                    rewind_viewer::colors::purple::Magenta, true);

  rc.circle({pos.x + 75.0, pos.y + 50.0}, 20.0, rewind_viewer::colors::purple::Magenta);
  rc.arc({pos.x + 75.0, pos.y + 50.0}, 17.5, 2.443, 3.84, rewind_viewer::colors::purple::Magenta);
  rc.arc({pos.x + 75.0, pos.y + 50.0}, 17.5, -1.745, 1.745, rewind_viewer::colors::purple::Magenta);
  rc.circle_segment({pos.x + 75.0, pos.y + 50.0}, 15.0, 2.443, 3.84,
                    rewind_viewer::colors::purple::Magenta);
  rc.circle_segment({pos.x + 75.0, pos.y + 50.0}, 15.0, -1.745, 1.745,
                    rewind_viewer::colors::purple::Magenta);

  // Not supported for permanent frames
  rc.popup_round({pos.x + 75.0, pos.y + 50.0}, 20.0, "Round popup #%d", i);

  rc.triangle({pos.x + 25.0, pos.y + 50.0}, {pos.x + 45.0, pos.y + 15.0},
              {pos.x + 5.0, pos.y + 15.0}, rewind_viewer::colors::blue::Cyan, true);
  rc.triangle({pos.x + 75.0, pos.y + 50.0}, {pos.x + 95.0, pos.y + 15.0},
              {pos.x + 55.0, pos.y + 15.0}, rewind_viewer::colors::blue::Cyan);
  rc.rectangle({pos.x + 10.0, pos.y + 40.0}, {20.0, 40.0}, rewind_viewer::colors::yellow::Yellow,
               true);
  rc.rectangle({pos.x + 60.0, pos.y + 40.0}, {20.0, 40.0}, rewind_viewer::colors::yellow::Yellow);

  // Not supported for permanent frames
  rc.popup({pos.x + 60.0, pos.y + 40.0}, {20.0, 40.0}, "Rectangular popup #%d", i);

  rc.line({pos.x + 5.0, pos.y + 5.0}, {pos.x + 95.0, pos.y + 95.0},
          rewind_viewer::colors::green::Green);
  rc.polyline({{pos.x + 5.0, pos.y + 95.0},
               {pos.x + 40.0, pos.y + 80.0},
               {pos.x + 60.0, pos.y + 20.0},
               {pos.x + 95.0, pos.y + 5.0}},
              rewind_viewer::colors::red::Crimson);
}

int main(int argc, char *argv[]) {
  // Should be just 1 instance.
  // Pass it via reference, shared_ptr ot create singleton to access it.
  // Assume rewind viewer is started on the same host with default port.
  rewind_viewer::RewindClient<Vec2D> rewind_client("127.0.0.1", 9111);

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
  for (int i = 0; i < 10; i++) {
    rewind_client.log_text("Iteration: %d", i);
    rewind_client.log_text("Extra message");
    rewind_client.log_text("Multi\nline\nmessage");

    rewind_client.switch_to_layer(0);
    rewind_client.tiles(start_pos, cell_size, row_size, &field_colors, false);

    // Normal layer
    rewind_client.switch_to_layer(i);
    rewind_client.set_opacity(255 - i * 15);
    pattern_position.x = static_cast<int>(i % 5) * 100.0;
    pattern_position.y = static_cast<int>(i / 5) * 200.0;
    draw_pattern(rewind_client, pattern_position, i);

    rewind_client.camera_view("Allways Camera", pattern_position, 100.0);

    // Permanent layer
    rewind_client.switch_to_layer(i, true);
    pattern_position.y += 100.0;
    draw_pattern(rewind_client, pattern_position, i);

    if (i % 2) {
      rewind_client.camera_view("Odd Camera", pattern_position, 100.0);
    }
    rewind_client.end_frame();
  }
  return EXIT_SUCCESS;
}

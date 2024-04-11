#include <rewind_viewer/RewindClient.h>
#include <rewind_viewer/colors.h>

// Your vector implementation, should have fields 'x', 'y'
struct Vec2D {
  double x;
  double y;
};

void draw_pattern(rewind_viewer::RewindClient<Vec2D> &rc, const Vec2D &pos, int i) {
  rc.circle({pos.x + 25.0, pos.y + 50.0}, 20.0, rewind_viewer::colors::purple::Magenta, true);
  rc.circle({pos.x + 75.0, pos.y + 50.0}, 20.0, rewind_viewer::colors::purple::Magenta);

  // Not supported for permanent frames
  rc.popup_round({pos.x + 75.0, pos.y + 50.0}, 20.0, "Round popup #%d", i);

  rc.triangle({pos.x + 25.0, pos.y + 50.0},
              {pos.x + 45.0, pos.y + 15.0},
              {pos.x + 5.0, pos.y + 15.0},
              rewind_viewer::colors::blue::Cyan,
              true);
  rc.triangle({pos.x + 75.0, pos.y + 50.0},
              {pos.x + 95.0, pos.y + 15.0},
              {pos.x + 55.0, pos.y + 15.0},
              rewind_viewer::colors::blue::Cyan);
  rc.rectangle({pos.x + 10.0, pos.y + 40.0},
               {20.0, 40.0},
               rewind_viewer::colors::yellow::Yellow,
               true);
  rc.rectangle({pos.x + 60.0, pos.y + 40.0},
               {20.0, 40.0},
               rewind_viewer::colors::yellow::Yellow);

  // Not supported for permanent frames
  rc.popup({pos.x + 60.0, pos.y + 40.0},
           {20.0, 40.0},
           "Rectangular popup #%d", i);

  rc.line({pos.x + 5.0, pos.y + 5.0},
          {pos.x + 95.0, pos.y + 95.0},
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

  rewind_client.map({500.0, 400.0}, 5, 4);

  Vec2D pattern_position{};
  for (int i = 0; i < 10; i++) {
    rewind_client.log_text("Iteration: %d", i);
    rewind_client.log_text("Extra message");
    rewind_client.log_text("Multi\nline\nmessage");

    // Normal layer
    rewind_client.switch_to_layer(i);
    rewind_client.set_opacity(255 - i * 15);
    pattern_position.x = static_cast<int>(i % 5) * 100.0;
    pattern_position.y = static_cast<int>(i / 5) * 200.0;
    draw_pattern(rewind_client, pattern_position, i);

    // Permanent layer
    rewind_client.switch_to_layer(i, true);
    pattern_position.y += 100.0;
    draw_pattern(rewind_client, pattern_position, i);

    rewind_client.end_frame();
  }
  return EXIT_SUCCESS;
}

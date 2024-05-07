from rewind_viewer.client import RewindClient
from rewind_viewer.colors import colors
import random
import sys


class Vec2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y


def draw_pattern(rc, pos, i):
    rc.mask_circle(Vec2D(pos.x + 15.0, pos.y + 75.0), 6.0)
    rc.mask_circle_segment(Vec2D(pos.x + 15.0, pos.y + 61.0), 6.0, 2.443, 3.84)
    rc.mask_circle_segment(Vec2D(pos.x + 15.0, pos.y + 61.0), 6.0, -1.745, 1.745)
    rc.mask_arc(Vec2D(pos.x + 15.0, pos.y + 47.0), 6.0, 2.443, 3.84)
    rc.mask_arc(Vec2D(pos.x + 15.0, pos.y + 47.0), 6.0, -1.745, 1.745)

    rc.mask_triangle(Vec2D(pos.x + 25.0, pos.y + 40.0), Vec2D(pos.x + 35.0, pos.y + 20.0),
                     Vec2D(pos.x + 15.0, pos.y + 20.0))
    rc.mask_rectangle(Vec2D(pos.x + 35.0, pos.y + 45.0), Vec2D(20.0, 20.0))

    rc.circle(Vec2D(pos.x + 25.0, pos.y + 50.0), 20.0, colors.purple.Magenta, True)
    rc.arc(Vec2D(pos.x + 50.0, pos.y + 12.0), 6.0, 2.443, 3.84, colors.purple.Magenta, True)
    rc.arc(Vec2D(pos.x + 50.0, pos.y + 12.0), 6.0, -1.745, 1.745, colors.purple.Magenta, True)
    rc.circle_segment(Vec2D(pos.x + 50.0, pos.y + 88.0), 6.0, 2.443, 3.84, colors.purple.Magenta, True)
    rc.circle_segment(Vec2D(pos.x + 50.0, pos.y + 88.0), 6.0, -1.745, 1.745, colors.purple.Magenta, True)

    rc.circle(Vec2D(pos.x + 75.0, pos.y + 50.0), 20.0, colors.purple.Magenta)
    rc.arc(Vec2D(pos.x + 75.0, pos.y + 50.0), 17.5, 2.443, 3.84, colors.purple.Magenta)
    rc.arc(Vec2D(pos.x + 75.0, pos.y + 50.0), 17.5, -1.745, 1.745, colors.purple.Magenta)
    rc.circle_segment(Vec2D(pos.x + 75.0, pos.y + 50.0), 15.0, 2.443, 3.84, colors.purple.Magenta)
    rc.circle_segment(Vec2D(pos.x + 75.0, pos.y + 50.0), 15.0, -1.745, 1.745, colors.purple.Magenta)

    # Not supported for permanent frames
    rc.popup_round(Vec2D(pos.x + 75.0, pos.y + 50.0), 20.0, f"Round popup #{i}")

    rc.triangle(Vec2D(pos.x + 25.0, pos.y + 50.0), Vec2D(pos.x + 45.0, pos.y + 15.0),
                Vec2D(pos.x + 5.0, pos.y + 15.0), colors.blue.Cyan, True)
    rc.triangle(Vec2D(pos.x + 75.0, pos.y + 50.0), Vec2D(pos.x + 95.0, pos.y + 15.0),
                Vec2D(pos.x + 55.0, pos.y + 15.0), colors.blue.Cyan)
    rc.rectangle(Vec2D(pos.x + 10.0, pos.y + 40.0), Vec2D(20.0, 40.0), colors.yellow.Yellow, True)
    rc.rectangle(Vec2D(pos.x + 60.0, pos.y + 40.0), Vec2D(20.0, 40.0), colors.yellow.Yellow)

    # Not supported for permanent frames
    rc.popup(Vec2D(pos.x + 60.0, pos.y + 40.0), Vec2D(20.0, 40.0), f"Rectangular popup #{i}")

    rc.line(Vec2D(pos.x + 5.0, pos.y + 5.0), Vec2D(pos.x + 95.0, pos.y + 95.0),
            colors.green.Green)
    rc.polyline((Vec2D(pos.x + 5.0, pos.y + 95.0),
                 Vec2D(pos.x + 40.0, pos.y + 80.0),
                 Vec2D(pos.x + 60.0, pos.y + 20.0),
                 Vec2D(pos.x + 95.0, pos.y + 5.0)),
                colors.red.Crimson)


def main():
    rewind_client = RewindClient("127.0.0.1", 9111)

    random.seed()  # Initialize the random number generator
    gen = random.Random()

    map_pos = Vec2D(0.0, 0.0)
    map_size = Vec2D(500.0, 400.0)
    rewind_client.map(map_pos, map_size, 5, 40)
    field_colors = []

    cell_size = Vec2D(5.0, 10.0)
    start_pos = Vec2D(0.0, 0.0)
    pos = Vec2D(0.0, 0.0)
    row_size = int(map_size.x / cell_size.x)
    while pos.y < map_size.y:
        pos.x = start_pos.x
        while pos.x < map_size.x:
            color = 127
            if pos.x < map_size.x * 0.5 or gen.randint(0, 23) != 4:
                color |= int(255.0 * (pos.y / map_size.y)) << 24
            color |= int(255.0 * (pos.y / map_size.y)) << 8
            color |= int(255.0 * (pos.x / map_size.x)) << 16
            field_colors.append(color)
            pos.x += cell_size.x
        pos.y += cell_size.y

    pattern_position = Vec2D(0.0, 0.0)
    for i in range(10):
        rewind_client.log_text(f"Iteration: {i}")
        rewind_client.log_text("Extra message")
        rewind_client.log_text("Multi\nline\nmessage")

        rewind_client.switch_to_layer(0)
        rewind_client.tiles(start_pos, cell_size, row_size, field_colors, False)

        rewind_client.switch_to_layer(i)
        rewind_client.set_opacity(255 - i * 15)
        pattern_position.x = (i % 5) * 100.0
        pattern_position.y = (i // 5) * 200.0
        draw_pattern(rewind_client, pattern_position, i)

        rewind_client.camera_view("Always Camera", pattern_position, 100.0)

        rewind_client.switch_to_layer(i, True)
        pattern_position.y += 100.0
        draw_pattern(rewind_client, pattern_position, i)

        if i % 2 == 1:
            rewind_client.camera_view("Odd Camera", pattern_position, 100.0)

        rewind_client.end_frame()

    return 0


if __name__ == "__main__":
    sys.exit(main())

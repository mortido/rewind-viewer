from rewind_viewer.client import RewindClient
from rewind_viewer.colors import colors
import random
import sys
import math


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


def draw_HUD(rc):
    """Draw HUD elements on permanent layers with different origins"""
    zero = Vec2D(0.0, 0.0)
    text_pos_1 = Vec2D(-40.0, 50.0)
    text_pos_2 = Vec2D(-40.0, 0.0)

    # Left top corner
    rc.set_layer(0, True, origin=1)  # left_top
    rc.circle(zero, 50, colors.red.Crimson, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Left center
    rc.set_layer(1, True, origin=2)  # left_center
    rc.circle(zero, 50, colors.pink.HotPink, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Left bottom
    rc.set_layer(2, True, origin=3)  # left_bottom
    rc.circle(zero, 50, colors.orange.Tomato, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Right top
    rc.set_layer(3, True, origin=4)  # right_top
    rc.circle(zero, 50, colors.yellow.Gold, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Right center
    rc.set_layer(4, True, origin=5)  # right_center
    rc.circle(zero, 50, colors.purple.Orchid, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Right bottom
    rc.set_layer(5, True, origin=6)  # right_bottom
    rc.circle(zero, 50, colors.green.LimeGreen, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Top center
    rc.set_layer(6, True, origin=7)  # top_center
    rc.circle(zero, 50, colors.blue.SkyBlue, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")

    # Bottom center
    rc.set_layer(7, True, origin=8)  # bottom_center
    rc.circle(zero, 50, colors.brown.Chocolate, True)
    rc.text(text_pos_1, 20.0, 0xFFFFFF00, ":horse::heart:")
    rc.text(text_pos_2, 20.0, 0xFFFFFF00, ":heart::unicorn_face:")


def create_star_proto(rc):
    """Create a reusable star proto"""
    rc.start_proto()
    # Draw a 5-pointed star
    points = []
    for i in range(10):
        angle = i * math.pi / 5.0 - math.pi / 2.0
        radius = 10.0 if i % 2 == 0 else 4.0
        points.append(Vec2D(math.cos(angle) * radius, math.sin(angle) * radius))

    for i in range(len(points)):
        next_i = (i + 1) % len(points)
        rc.line(points[i], points[next_i], colors.yellow.Gold)

    return rc.end_proto()


def setup(rc):
    """Setup layer names"""
    rc.set_layer_name(0, "layer 1")
    rc.set_layer_name(1, "layer 2")
    rc.set_layer_name(2, "layer 3")
    rc.set_layer_name(3, "layer 4")
    rc.set_layer_name(4, "layer 5")
    rc.set_layer_name(5, "layer 6")
    rc.set_layer_name(9, "layer 10")
    rc.set_layer_name(9, "Perma layer 10", True)


def main():
    rewind_client = RewindClient("127.0.0.1", 9111)

    setup(rewind_client)

    random.seed()  # Initialize the random number generator
    gen = random.Random()

    # Optional: Uncomment to create UI actions for interactive control
    # rewind_client.create_button_action("Reset View")
    # rewind_client.create_bool_input_action("Show Labels", True)
    # rewind_client.create_int_input_action("Speed", 5, 1, 10)
    # rewind_client.create_float_input_action("Opacity", 0.8, 0.0, 1.0)
    # rewind_client.create_select_input_action("Mode", ["Normal", "Debug", "Performance"], 0)
    # rewind_client.create_string_input_action("Team Name", "Player 1")

    # Optional: Uncomment to subscribe to keyboard events
    # rewind_client.subscribe('r', "Reset", continuous=False, capture_mouse=False)
    # rewind_client.subscribe('m', "Mouse", continuous=True, capture_mouse=True)

    map_pos = Vec2D(-100.0, -100.0)
    map_size = Vec2D(500.0, 400.0)
    rewind_client.map(map_pos, map_size, 5, 40)
    field_colors = []

    cell_size = Vec2D(5.0, 10.0)
    start_pos = map_pos
    pos = Vec2D(map_pos.x, map_pos.y)
    row_size = int(map_size.x / cell_size.x)
    while pos.y < map_pos.y + map_size.y:
        pos.x = start_pos.x
        while pos.x < map_pos.x + map_size.x:
            color = 127
            pos_on_map_x = pos.x - map_pos.x
            pos_on_map_y = pos.y - map_pos.y
            if pos_on_map_x < map_size.x * 0.5 or gen.randint(0, 23) != 4:
                color |= int(255.0 * (pos_on_map_y / map_size.y)) << 24
            color |= int(255.0 * (pos_on_map_y / map_size.y)) << 8
            color |= int(255.0 * (pos_on_map_x / map_size.x)) << 16
            field_colors.append(color)
            pos.x += cell_size.x
        pos.y += cell_size.y

    # Set the initial layer to permanent and draw the field once
    rewind_client.set_layer(0, True)
    rewind_client.tiles(start_pos, cell_size, row_size, field_colors, False)

    # Draw HUD elements
    draw_HUD(rewind_client)

    # NEW: Create a reusable star proto
    star_proto_id = create_star_proto(rewind_client)

    pattern_position = Vec2D(0.0, 0.0)
    for i in range(10):
        rewind_client.log_text(f"Iteration: {i}")
        rewind_client.log_text("Extra message")
        rewind_client.log_text("Multi\nline\nmessage")
        rewind_client.log_text(":horse: = :heart:!")

        # Optional: Uncomment to read events and actions from UI
        # Note: This will block waiting for server response, only use in interactive mode
        # try:
        #     events_data = rewind_client.read_events()
        #     if events_data.get('events'):
        #         rewind_client.log_text(f"Events triggered: {events_data['events']}")
        #     if events_data.get('actions'):
        #         rewind_client.log_text(f"Actions triggered: {events_data['actions']}")
        # except Exception as e:
        #     # Skip if server doesn't respond
        #     pass

        # Normal layer
        rewind_client.set_layer(i)
        opacity = 255 - i * 15
        rewind_client.set_opacity(opacity)

        # Draw pattern on normal layer
        pattern_position.x = map_pos.x + (i % 5) * 100.0
        pattern_position.y = map_pos.y + (i // 5) * 200.0
        draw_pattern(rewind_client, pattern_position, i)

        # Add text with emoji (using unicode emoji directly or emoji shortcodes)
        rewind_client.text(pattern_position, 10.0, 0xFFFF00 | (opacity << 24), "Hello, 🐴!")

        rewind_client.camera_view("Allways Camera", pattern_position, 100.0)

        # Permanent layer
        rewind_client.set_layer(i, True)
        pattern_position.y += 100.0
        draw_pattern(rewind_client, pattern_position, i)

        if i % 2:
            rewind_client.camera_view("Odd Camera", pattern_position, 100.0)

        rewind_client.end_frame()

    # Optional: Uncomment to clean up events and actions if you enabled them above
    # rewind_client.unsubscribe('r')
    # rewind_client.unsubscribe('m')
    # rewind_client.remove_action("Reset View")
    # rewind_client.remove_action("Show Labels")
    # rewind_client.remove_action("Speed")
    # rewind_client.remove_action("Opacity")
    # rewind_client.remove_action("Mode")
    # rewind_client.remove_action("Team Name")

    return 0


if __name__ == "__main__":
    sys.exit(main())

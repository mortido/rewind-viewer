import socket
import ujson
import struct


class RewindClient:
    MESSAGE_SCHEMA_VERSION = 6
    MAX_MESSAGE_SIZE = 1024 * 1024  # 1MB

    def __init__(self, host, port):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self._socket.connect((host, port))

        self._opacity = 0xFF000000
        self._proto_id = 0
        self._send_protocol_version()

    def set_opacity(self, opacity):
        if opacity > 255:
            raise RuntimeError("Opacity value should be no more than 255")
        self._opacity = (int(opacity) << 24)

    def _send_protocol_version(self):
        self._socket.sendall(struct.pack('<H', self.MESSAGE_SCHEMA_VERSION))

    def _send_bytes(self, data):
        if len(data) > self.MAX_MESSAGE_SIZE:
            raise RuntimeError("Rewind message size can't be more than 1MB")
        self._socket.sendall(struct.pack('<I', len(data)) + data)

    def _send_command(self, command_type, command_data):
        message = {
            'cmd': {
                't': command_type,
                'd': command_data
            }
        }
        data = ujson.dumps(message).encode('utf-8')
        self._send_bytes(data)

    def circle(self, center, radius, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'ct': {'x': center.x, 'y': center.y},
            'r': radius
        }
        self._send_command('C', command_data)

    def circle_segment(self, center, radius, start_angle, end_angle, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'ct': {'x': center.x, 'y': center.y},
            'r': radius,
            'sa': start_angle,
            'ea': end_angle
        }
        self._send_command('CS', command_data)

    def arc(self, center, radius, start_angle, end_angle, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'ct': {'x': center.x, 'y': center.y},
            'r': radius,
            'sa': start_angle,
            'ea': end_angle
        }
        self._send_command('A', command_data)

    def rectangle(self, position, size, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'p': {'x': position.x, 'y': position.y},
            's': {'x': size.x, 'y': size.y}
        }
        self._send_command('R', command_data)

    def line(self, p1, p2, color):
        self.polyline([p1, p2], color)

    def polyline(self, points, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'pts': [{'x': point.x, 'y': point.y} for point in points]
        }
        self._send_command('P', command_data)

    def triangle(self, p1, p2, p3, color, fill=False):
        command_data = {
            'c': {'v': color | self._opacity, 'f': fill},
            'pts': [
                {'x': p1.x, 'y': p1.y},
                {'x': p2.x, 'y': p2.y},
                {'x': p3.x, 'y': p3.y}
            ]
        }
        self._send_command('TR', command_data)

    def tiles(self, position, cell_size, row_size, colors, use_global_alpha=True):
        if use_global_alpha:
            colors = [(color & 0xFFFFFF) | self._opacity for color in colors]
        command_data = {
            'p': {'x': position.x, 'y': position.y},
            's': {'x': cell_size.x, 'y': cell_size.y},
            'rs': row_size,
            'clrs': colors
        }
        self._send_command('CF', command_data)

    def log_text(self, text):
        command_data = {'t': text}
        self._send_command('LT', command_data)

    def popup(self, position, size, text):
        command_data = {
            't': text,
            'ap': {'x': position.x, 'y': position.y},
            'as': {'x': size.x, 'y': size.y}
        }
        self._send_command('PP', command_data)

    def popup_round(self, center, radius, text):
        command_data = {
            't': text,
            'ac': {'x': center.x, 'y': center.y},
            'r': radius
        }
        self._send_command('PR', command_data)

    def camera_view(self, name, position, radius):
        command_data = {
            'n': name,
            'p': {'x': position.x, 'y': position.y},
            'vr': radius
        }
        self._send_command('CV', command_data)

    def set_layer(self, layer_id, permanent=False, origin=None):
        """Set the current layer.

        Args:
            layer_id: Layer identifier
            permanent: Whether this layer is permanent
            origin: Camera origin (0=game, 1=left_top, 2=left_center, 3=left_bottom,
                    4=right_top, 5=right_center, 6=right_bottom, 7=top_center, 8=bottom_center)
        """
        layer_data = {'i': layer_id, 'upf': permanent}
        if origin is not None:
            layer_data['o'] = origin
        command_data = {'l': layer_data}
        self._send_command('O', command_data)

    def set_layer_name(self, layer_id, name, permanent=False):
        """Set the name of a layer.

        Args:
            layer_id: Layer identifier
            name: Name for the layer
            permanent: Whether this layer is permanent
        """
        command_data = {
            'l': {'i': layer_id, 'upf': permanent, 'n': name}
        }
        self._send_command('O', command_data)

    def map(self, position, size, grid_x, grid_y):
        command_data = {
            'm': {
                'p': {'x': position.x, 'y': position.y},
                's': {'x': size.x, 'y': size.y},
                'xg': grid_x,
                'yg': grid_y
            }
        }
        self._send_command('O', command_data)

    def mask_circle(self, center, radius):
        command_data = {
            'ct': {'x': center.x, 'y': center.y},
            'r': radius
        }
        self._send_command('C', command_data)

    def mask_arc(self, center, radius, start_angle, end_angle):
        command_data = {
            'ct': {'x': center.x, 'y': center.y},
            'r': radius,
            'sa': start_angle,
            'ea': end_angle
        }
        self._send_command('A', command_data)

    def mask_circle_segment(self, center, radius, start_angle, end_angle):
        command_data = {
            'ct': {'x': center.x, 'y': center.y},
            'r': radius,
            'sa': start_angle,
            'ea': end_angle
        }
        self._send_command('CS', command_data)

    def mask_triangle(self, p1, p2, p3):
        command_data = {
            'pts': [
                {'x': p1.x, 'y': p1.y},
                {'x': p2.x, 'y': p2.y},
                {'x': p3.x, 'y': p3.y}
            ]
        }
        self._send_command('TR', command_data)

    def mask_rectangle(self, position, size):
        command_data = {
            'p': {'x': position.x, 'y': position.y},
            's': {'x': size.x, 'y': size.y}
        }
        self._send_command('R', command_data)

    def mask_polygon(self, points):
        command_data = {
            'pts': [{'x': point.x, 'y': point.y} for point in points]
        }
        self._send_command('P', command_data)

    def text(self, position, size, color, text):
        """Draw text at the specified position with given size and color."""
        if size <= 0:
            raise ValueError(f"Text size should be positive, got {size}")
        command_data = {
            'p': {'x': position.x, 'y': position.y},
            's': size,
            'c': color | self._opacity,
            't': text
        }
        self._send_command('T', command_data)

    def start_proto(self):
        """Start defining a proto (reusable primitive group)."""
        self._send_command('SP', {})

    def end_proto(self):
        """End the current proto definition and return its ID."""
        self._send_command('EP', {})
        proto_id = self._proto_id
        self._proto_id += 1
        return proto_id

    def draw_proto(self, proto_id, position, angle=0.0, color=0, scale=1.0):
        """Draw a previously defined proto at the specified position.

        Args:
            proto_id: ID of the proto to draw (returned by end_proto)
            position: Position to draw the proto
            angle: Rotation angle in radians
            color: Color tint to apply
            scale: Scale factor
        """
        command_data = {
            'id': proto_id,
            'p': {'x': position.x, 'y': position.y},
            'a': angle,
            'c': color,
            'sc': scale
        }
        self._send_command('DP', command_data)

    def subscribe(self, key, name, continuous=False, capture_mouse=False):
        """Subscribe to keyboard or mouse events.

        Args:
            key: Character key to subscribe to (e.g., 'a', 'b')
            name: Name for this event subscription
            continuous: If True, fires continuously while key is held
            capture_mouse: If True, captures mouse position events
        """
        command_data = {
            'k': ord(key) if isinstance(key, str) else key,
            'n': name,
            'c': continuous,
            'cm': capture_mouse
        }
        self._send_command('S', command_data)

    def unsubscribe(self, key):
        """Unsubscribe from keyboard or mouse events.

        Args:
            key: Character key to unsubscribe from
        """
        command_data = {
            'k': ord(key) if isinstance(key, str) else key
        }
        self._send_command('US', command_data)

    def read_events(self):
        """Read triggered events and actions from the server.

        Returns:
            Dictionary with 'events' and 'actions' keys containing triggered events
        """
        self._send_command('RE', {})

        # Read response from server
        size_data = self._socket.recv(4)
        if len(size_data) < 4:
            return {'events': [], 'actions': []}

        message_size = struct.unpack('<I', size_data)[0]
        if message_size > self.MAX_MESSAGE_SIZE:
            raise RuntimeError(f"Message size {message_size} exceeds maximum {self.MAX_MESSAGE_SIZE}")

        response_data = b''
        while len(response_data) < message_size:
            chunk = self._socket.recv(min(message_size - len(response_data), 4096))
            if not chunk:
                break
            response_data += chunk

        if len(response_data) < message_size:
            raise RuntimeError("Incomplete response from server")

        return ujson.loads(response_data.decode('utf-8'))

    def create_button_action(self, name):
        """Create a button action in the UI.

        Args:
            name: Name of the button action
        """
        command_data = {
            'n': name,
            'it': 'button',
            'id': {}
        }
        self._send_command('CA', command_data)

    def create_int_input_action(self, name, default_value=0, min_value=0, max_value=100):
        """Create an integer input action in the UI.

        Args:
            name: Name of the input action
            default_value: Default integer value
            min_value: Minimum allowed value
            max_value: Maximum allowed value
        """
        command_data = {
            'n': name,
            'it': 'int',
            'id': {
                'dv': default_value,
                'min': min_value,
                'max': max_value
            }
        }
        self._send_command('CA', command_data)

    def create_float_input_action(self, name, default_value=0.0, min_value=0.0, max_value=1.0):
        """Create a float input action in the UI.

        Args:
            name: Name of the input action
            default_value: Default float value
            min_value: Minimum allowed value
            max_value: Maximum allowed value
        """
        command_data = {
            'n': name,
            'it': 'float',
            'id': {
                'dv': default_value,
                'min': min_value,
                'max': max_value
            }
        }
        self._send_command('CA', command_data)

    def create_select_input_action(self, name, options, selected_option=0):
        """Create a select/dropdown input action in the UI.

        Args:
            name: Name of the select action
            options: List of string options
            selected_option: Index of initially selected option
        """
        command_data = {
            'n': name,
            'it': 'select',
            'id': {
                'opts': options,
                'so': selected_option
            }
        }
        self._send_command('CA', command_data)

    def create_string_input_action(self, name, default_value=""):
        """Create a string input action in the UI.

        Args:
            name: Name of the input action
            default_value: Default string value
        """
        command_data = {
            'n': name,
            'it': 'string',
            'id': {
                'dv': default_value
            }
        }
        self._send_command('CA', command_data)

    def create_bool_input_action(self, name, default_value=False):
        """Create a boolean checkbox action in the UI.

        Args:
            name: Name of the checkbox action
            default_value: Default boolean value
        """
        command_data = {
            'n': name,
            'it': 'bool',
            'id': {
                'dv': default_value
            }
        }
        self._send_command('CA', command_data)

    def remove_action(self, name):
        """Remove an action from the UI.

        Args:
            name: Name of the action to remove
        """
        command_data = {
            'n': name
        }
        self._send_command('RA', command_data)

    def end_frame(self):
        self._send_command('EF', {})

    def close(self):
        try:
            self._socket.shutdown(socket.SHUT_RDWR)
        except socket.error as e:
            print(f"Error shutting down socket: {e}")
        finally:
            self._socket.close()

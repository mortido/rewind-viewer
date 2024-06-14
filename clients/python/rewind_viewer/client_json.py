import socket
import json
import struct


class RewindClient:
    MESSAGE_SCHEMA_VERSION = 6
    MAX_MESSAGE_SIZE = 1024 * 1024  # 1MB

    def __init__(self, host, port):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self._socket.connect((host, port))

        self._opacity = 0xFF000000
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
        data = json.dumps(message).encode('utf-8')
        self._send_bytes(data)

    def circle(self, center, radius, color, fill=False):
        command_data = {
            'c': {'v': color, 'f': fill},
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
            'c': {'v': color, 'f': fill},
            'ct': {'x': center.x, 'y': center.y},
            'r': radius,
            'sa': start_angle,
            'ea': end_angle
        }
        self._send_command('A', command_data)

    def rectangle(self, position, size, color, fill=False):
        command_data = {
            'c': {'v': color, 'f': fill},
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

    def set_layer(self, layer_id, permanent=False):
        command_data = {
            'l': {'i': layer_id, 'upf': permanent}
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

    def end_frame(self):
        self._send_command('EF', {})

    def close(self):
        try:
            self._socket.shutdown(socket.SHUT_RDWR)
        except socket.error as e:
            print(f"Error shutting down socket: {e}")
        finally:
            self._socket.close()

import socket
import struct
from flatbuffers import Builder

# import os, sys
# sys.path.insert(0, os.path.dirname(__file__))

from rewind_viewer.fbs.Arc import *
from rewind_viewer.fbs.CameraView import *
from rewind_viewer.fbs.Circle import *
from rewind_viewer.fbs.CircleSegment import *
from rewind_viewer.fbs.Color import *
from rewind_viewer.fbs.Command import *
from rewind_viewer.fbs.EndFrame import *
from rewind_viewer.fbs.Layer import *
from rewind_viewer.fbs.LogText import *
from rewind_viewer.fbs.Map import *
from rewind_viewer.fbs.Options import *
from rewind_viewer.fbs.Polyline import *
from rewind_viewer.fbs.Popup import *
from rewind_viewer.fbs.PopupRound import *
from rewind_viewer.fbs.Rectangle import *
from rewind_viewer.fbs.RewindMessage import *
from rewind_viewer.fbs.Tiles import *
from rewind_viewer.fbs.Triangle import *
from rewind_viewer.fbs.Vector2f import *


class RewindClient:
    MESSAGE_SCHEMA_VERSION = 5
    MAX_MESSAGE_SIZE = 1024 * 1024  # 1MB

    def __init__(self, host, port):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self._socket.connect((host, port))

        self._builder = Builder(0)
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

    def circle(self, center, radius, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        CircleStart(self._builder)
        CircleAddColor(self._builder, color_obj)
        CircleAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        CircleAddRadius(self._builder, radius)
        command = CircleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Circle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def circle_segment(self, center, radius, start_angle, end_angle, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color | self._opacity)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        CircleSegmentStart(self._builder)
        CircleSegmentAddColor(self._builder, color_obj)
        CircleSegmentAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        CircleSegmentAddRadius(self._builder, radius)
        CircleSegmentAddStartAngle(self._builder, start_angle)
        CircleSegmentAddEndAngle(self._builder, end_angle)
        command = CircleSegmentEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.CircleSegment)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def arc(self, center, radius, start_angle, end_angle, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        ArcStart(self._builder)
        ArcAddColor(self._builder, color_obj)
        ArcAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        ArcAddRadius(self._builder, radius)
        ArcAddStartAngle(self._builder, start_angle)
        ArcAddEndAngle(self._builder, end_angle)
        command = ArcEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Arc)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def rectangle(self, position, size, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        RectangleStart(self._builder)
        RectangleAddColor(self._builder, color_obj)
        RectangleAddPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        RectangleAddSize(self._builder, CreateVector2f(self._builder, size.x, size.y))
        command = RectangleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Rectangle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def line(self, p1, p2, color):
        self.polyline((p1, p2), color)

    def polyline(self, points, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color | self._opacity)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        PolylineStartPointsVector(self._builder, len(points))
        for point in reversed(points):
            CreateVector2f(self._builder, point.x, point.y)
        points_vector = self._builder.EndVector(len(points))

        PolylineStart(self._builder)
        PolylineAddColor(self._builder, color_obj)
        PolylineAddPoints(self._builder, points_vector)
        command = PolylineEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Polyline)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def triangle(self, p1, p2, p3, color, fill=False):
        self._builder.Clear()
        ColorStart(self._builder)
        ColorAddValue(self._builder, color | self._opacity)
        ColorAddFill(self._builder, fill)
        color_obj = ColorEnd(self._builder)
        TriangleStartPointsVector(self._builder, 3)
        # Insert points in reverse order for flatbuffers
        CreateVector2f(self._builder, p3.x, p3.y)
        CreateVector2f(self._builder, p2.x, p2.y)
        CreateVector2f(self._builder, p1.x, p1.y)
        points_vector = self._builder.EndVector(3)

        TriangleStart(self._builder)
        TriangleAddColor(self._builder, color_obj)
        TriangleAddPoints(self._builder, points_vector)
        command = TriangleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Triangle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def tiles(self, position, cell_size, row_size, colors, use_global_alpha=True):
        self._builder.Clear()
        if use_global_alpha:
            colors = [(color & 0xFFFFFF) | self._opacity for color in colors]
        TilesStartColorsVector(self._builder, len(colors))
        for color in reversed(colors):
            self._builder.PrependUint32(color)
        colors_vector = self._builder.EndVector(len(colors))

        TilesStart(self._builder)
        TilesAddPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        TilesAddCellSize(self._builder, CreateVector2f(self._builder, cell_size.x, cell_size.y))
        TilesAddRowSize(self._builder, row_size)
        TilesAddColors(self._builder, colors_vector)
        command = TilesEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Tiles)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def log_text(self, text):
        self._builder.Clear()
        text_obj = self._builder.CreateString(text)
        LogTextStart(self._builder)
        LogTextAddText(self._builder, text_obj)
        command = LogTextEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.LogText)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def popup(self, position, size, text):
        self._builder.Clear()
        text_obj = self._builder.CreateString(text)
        PopupStart(self._builder)
        PopupAddAreaPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        PopupAddAreaSize(self._builder, CreateVector2f(self._builder, size.x, size.y))
        PopupAddText(self._builder, text_obj)
        command = PopupEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Popup)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def popup_round(self, center, radius, text):
        self._builder.Clear()
        text_obj = self._builder.CreateString(text)
        PopupRoundStart(self._builder)
        PopupRoundAddAreaCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        PopupRoundAddAreaRadius(self._builder, radius)
        PopupRoundAddText(self._builder, text_obj)
        command = PopupRoundEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.PopupRound)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def camera_view(self, name, position, radius):
        self._builder.Clear()
        name_obj = self._builder.CreateString(name)
        CameraViewStart(self._builder)
        CameraViewAddName(self._builder, name_obj)
        CameraViewAddPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        CameraViewAddViewRadius(self._builder, radius)
        command = CameraViewEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.CameraView)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def switch_to_layer(self, layer_id, permanent=False):
        self._builder.Clear()
        LayerStart(self._builder)
        LayerAddId(self._builder, layer_id)
        LayerAddUsePermanentFrame(self._builder, permanent)
        layer_command = LayerEnd(self._builder)

        OptionsStart(self._builder)
        OptionsAddLayer(self._builder, layer_command)
        command = OptionsEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Options)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def map(self, position, size, grid_x, grid_y):
        self._builder.Clear()
        # position_obj =
        # size_obj =
        MapStart(self._builder)
        MapAddPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        MapAddSize(self._builder, CreateVector2f(self._builder, size.x, size.y))
        MapAddXGrid(self._builder, grid_x)
        MapAddYGrid(self._builder, grid_y)
        map_command = MapEnd(self._builder)

        OptionsStart(self._builder)
        OptionsAddMap(self._builder, map_command)
        command = OptionsEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Options)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_circle(self, center, radius):
        self._builder.Clear()
        CircleStart(self._builder)
        CircleAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        CircleAddRadius(self._builder, radius)
        command = CircleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Circle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_arc(self, center, radius, start_angle, end_angle):
        self._builder.Clear()
        ArcStart(self._builder)
        ArcAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        ArcAddRadius(self._builder, radius)
        ArcAddStartAngle(self._builder, start_angle)
        ArcAddEndAngle(self._builder, end_angle)
        command = ArcEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Arc)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_circle_segment(self, center, radius, start_angle, end_angle):
        self._builder.Clear()
        CircleSegmentStart(self._builder)
        CircleSegmentAddCenter(self._builder, CreateVector2f(self._builder, center.x, center.y))
        CircleSegmentAddRadius(self._builder, radius)
        CircleSegmentAddStartAngle(self._builder, start_angle)
        CircleSegmentAddEndAngle(self._builder, end_angle)
        command = CircleSegmentEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.CircleSegment)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_triangle(self, p1, p2, p3):
        self._builder.Clear()
        TriangleStartPointsVector(self._builder, 3)
        # Insert points in reverse order for flatbuffers
        CreateVector2f(self._builder, p3.x, p3.y)
        CreateVector2f(self._builder, p2.x, p2.y)
        CreateVector2f(self._builder, p1.x, p1.y)
        points_vector = self._builder.EndVector(3)

        TriangleStart(self._builder)
        TriangleAddPoints(self._builder, points_vector)
        command = TriangleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Triangle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_rectangle(self, position, size):
        self._builder.Clear()
        RectangleStart(self._builder)
        RectangleAddPosition(self._builder, CreateVector2f(self._builder, position.x, position.y))
        RectangleAddSize(self._builder, CreateVector2f(self._builder, size.x, size.y))
        command = RectangleEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Rectangle)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def mask_polygon(self, points):
        self._builder.Clear()
        PolylineStartPointsVector(self._builder, len(points))
        for point in reversed(points):
            CreateVector2f(self._builder, point.x, point.y)
        points_vector = self._builder.EndVector(len(points))

        PolylineStart(self._builder)
        PolylineAddPoints(self._builder, points_vector)
        command = PolylineEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.Polyline)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def end_frame(self):
        self._builder.Clear()
        EndFrameStart(self._builder)
        command = EndFrameEnd(self._builder)

        RewindMessageStart(self._builder)
        RewindMessageAddCommandType(self._builder, Command.EndFrame)
        RewindMessageAddCommand(self._builder, command)
        msg = RewindMessageEnd(self._builder)
        self._builder.Finish(msg)
        self._send_bytes(self._builder.Output())

    def close(self):
        try:
            self._socket.shutdown(socket.SHUT_RDWR)
        except socket.error as e:
            print(f"Error shutting down socket: {e}")
        finally:
            self._socket.close()

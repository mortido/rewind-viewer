import socket
import struct
from flatbuffers import Builder

# TODO: remove before release
import os, sys
sys.path.insert(0, os.path.dirname(__file__))

from fbs.Arc import *
from fbs.CameraView import *
from fbs.Circle import *
from fbs.CircleSegment import *
from fbs.Color import *
from fbs.Command import *
from fbs.EndFrame import *
from fbs.Layer import *
from fbs.LogText import *
from fbs.Map import *
from fbs.Options import *
from fbs.Polyline import *
from fbs.Popup import *
from fbs.PopupRound import *
from fbs.Rectangle import *
from fbs.RewindMessage import *
from fbs.Tiles import *
from fbs.Triangle import *
from fbs.Vector2f import *


class RewindClient:
    MESSAGE_SCHEMA_VERSION = 5
    MAX_MESSAGE_SIZE = 1024 * 1024  # 1MB

    def __init__(self, host, port):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        self._socket.connect((host, port))

        self._builder = Builder(0)
        self._opacity = 0xFF000000
        self._send_schema_version()

    def set_opacity(self, opacity):
        if opacity > 255:
            raise RuntimeError("Opacity value should be no more than 255")
        self._opacity = (int(opacity) << 24)

    def _send_schema_version(self):
        self._socket.sendall(struct.pack('<H', self.MESSAGE_SCHEMA_VERSION))

    def _send_bytes(self, data):
        if len(data) > self.MAX_MESSAGE_SIZE:
            raise RuntimeError("Rewind message size can't be more than 1MB")
        self._socket.sendall(struct.pack('<I', len(data)) + data)

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

# automatically generated by the FlatBuffers compiler, do not modify

# namespace: fbs

import flatbuffers
from flatbuffers.compat import import_numpy
np = import_numpy()

class Map(object):
    __slots__ = ['_tab']

    @classmethod
    def GetRootAs(cls, buf, offset=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, offset)
        x = Map()
        x.Init(buf, n + offset)
        return x

    @classmethod
    def GetRootAsMap(cls, buf, offset=0):
        """This method is deprecated. Please switch to GetRootAs."""
        return cls.GetRootAs(buf, offset)
    # Map
    def Init(self, buf, pos):
        self._tab = flatbuffers.table.Table(buf, pos)

    # Map
    def Width(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(4))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float32Flags, o + self._tab.Pos)
        return 0.0

    # Map
    def Height(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(6))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Float32Flags, o + self._tab.Pos)
        return 0.0

    # Map
    def XGrid(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(8))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Uint32Flags, o + self._tab.Pos)
        return 0

    # Map
    def YGrid(self):
        o = flatbuffers.number_types.UOffsetTFlags.py_type(self._tab.Offset(10))
        if o != 0:
            return self._tab.Get(flatbuffers.number_types.Uint32Flags, o + self._tab.Pos)
        return 0

def MapStart(builder):
    builder.StartObject(4)

def Start(builder):
    MapStart(builder)

def MapAddWidth(builder, width):
    builder.PrependFloat32Slot(0, width, 0.0)

def AddWidth(builder, width):
    MapAddWidth(builder, width)

def MapAddHeight(builder, height):
    builder.PrependFloat32Slot(1, height, 0.0)

def AddHeight(builder, height):
    MapAddHeight(builder, height)

def MapAddXGrid(builder, xGrid):
    builder.PrependUint32Slot(2, xGrid, 0)

def AddXGrid(builder, xGrid):
    MapAddXGrid(builder, xGrid)

def MapAddYGrid(builder, yGrid):
    builder.PrependUint32Slot(3, yGrid, 0)

def AddYGrid(builder, yGrid):
    MapAddYGrid(builder, yGrid)

def MapEnd(builder):
    return builder.EndObject()

def End(builder):
    return MapEnd(builder)


class MapT(object):

    # MapT
    def __init__(self):
        self.width = 0.0  # type: float
        self.height = 0.0  # type: float
        self.xGrid = 0  # type: int
        self.yGrid = 0  # type: int

    @classmethod
    def InitFromBuf(cls, buf, pos):
        map = Map()
        map.Init(buf, pos)
        return cls.InitFromObj(map)

    @classmethod
    def InitFromPackedBuf(cls, buf, pos=0):
        n = flatbuffers.encode.Get(flatbuffers.packer.uoffset, buf, pos)
        return cls.InitFromBuf(buf, pos+n)

    @classmethod
    def InitFromObj(cls, map):
        x = MapT()
        x._UnPack(map)
        return x

    # MapT
    def _UnPack(self, map):
        if map is None:
            return
        self.width = map.Width()
        self.height = map.Height()
        self.xGrid = map.XGrid()
        self.yGrid = map.YGrid()

    # MapT
    def Pack(self, builder):
        MapStart(builder)
        MapAddWidth(builder, self.width)
        MapAddHeight(builder, self.height)
        MapAddXGrid(builder, self.xGrid)
        MapAddYGrid(builder, self.yGrid)
        map = MapEnd(builder)
        return map

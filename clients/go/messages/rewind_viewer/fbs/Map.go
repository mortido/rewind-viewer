// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type MapT struct {
	Width float32 `json:"width"`
	Height float32 `json:"height"`
	XGrid uint32 `json:"x_grid"`
	YGrid uint32 `json:"y_grid"`
}

func (t *MapT) Pack(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	if t == nil {
		return 0
	}
	MapStart(builder)
	MapAddWidth(builder, t.Width)
	MapAddHeight(builder, t.Height)
	MapAddXGrid(builder, t.XGrid)
	MapAddYGrid(builder, t.YGrid)
	return MapEnd(builder)
}

func (rcv *Map) UnPackTo(t *MapT) {
	t.Width = rcv.Width()
	t.Height = rcv.Height()
	t.XGrid = rcv.XGrid()
	t.YGrid = rcv.YGrid()
}

func (rcv *Map) UnPack() *MapT {
	if rcv == nil {
		return nil
	}
	t := &MapT{}
	rcv.UnPackTo(t)
	return t
}

type Map struct {
	_tab flatbuffers.Table
}

func GetRootAsMap(buf []byte, offset flatbuffers.UOffsetT) *Map {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &Map{}
	x.Init(buf, n+offset)
	return x
}

func FinishMapBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsMap(buf []byte, offset flatbuffers.UOffsetT) *Map {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &Map{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedMapBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *Map) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *Map) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *Map) Width() float32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetFloat32(o + rcv._tab.Pos)
	}
	return 0.0
}

func (rcv *Map) MutateWidth(n float32) bool {
	return rcv._tab.MutateFloat32Slot(4, n)
}

func (rcv *Map) Height() float32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.GetFloat32(o + rcv._tab.Pos)
	}
	return 0.0
}

func (rcv *Map) MutateHeight(n float32) bool {
	return rcv._tab.MutateFloat32Slot(6, n)
}

func (rcv *Map) XGrid() uint32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(8))
	if o != 0 {
		return rcv._tab.GetUint32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *Map) MutateXGrid(n uint32) bool {
	return rcv._tab.MutateUint32Slot(8, n)
}

func (rcv *Map) YGrid() uint32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(10))
	if o != 0 {
		return rcv._tab.GetUint32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *Map) MutateYGrid(n uint32) bool {
	return rcv._tab.MutateUint32Slot(10, n)
}

func MapStart(builder *flatbuffers.Builder) {
	builder.StartObject(4)
}
func MapAddWidth(builder *flatbuffers.Builder, width float32) {
	builder.PrependFloat32Slot(0, width, 0.0)
}
func MapAddHeight(builder *flatbuffers.Builder, height float32) {
	builder.PrependFloat32Slot(1, height, 0.0)
}
func MapAddXGrid(builder *flatbuffers.Builder, xGrid uint32) {
	builder.PrependUint32Slot(2, xGrid, 0)
}
func MapAddYGrid(builder *flatbuffers.Builder, yGrid uint32) {
	builder.PrependUint32Slot(3, yGrid, 0)
}
func MapEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

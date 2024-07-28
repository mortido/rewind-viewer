// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type ColorPoint struct {
	_tab flatbuffers.Table
}

func GetRootAsColorPoint(buf []byte, offset flatbuffers.UOffsetT) *ColorPoint {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &ColorPoint{}
	x.Init(buf, n+offset)
	return x
}

func FinishColorPointBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsColorPoint(buf []byte, offset flatbuffers.UOffsetT) *ColorPoint {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &ColorPoint{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedColorPointBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *ColorPoint) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *ColorPoint) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *ColorPoint) Color() uint32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetUint32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *ColorPoint) MutateColor(n uint32) bool {
	return rcv._tab.MutateUint32Slot(4, n)
}

func (rcv *ColorPoint) Position(obj *Vector2f) *Vector2f {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		x := o + rcv._tab.Pos
		if obj == nil {
			obj = new(Vector2f)
		}
		obj.Init(rcv._tab.Bytes, x)
		return obj
	}
	return nil
}

func ColorPointStart(builder *flatbuffers.Builder) {
	builder.StartObject(2)
}
func ColorPointAddColor(builder *flatbuffers.Builder, color uint32) {
	builder.PrependUint32Slot(0, color, 0)
}
func ColorPointAddPosition(builder *flatbuffers.Builder, position flatbuffers.UOffsetT) {
	builder.PrependStructSlot(1, flatbuffers.UOffsetT(position), 0)
}
func ColorPointEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

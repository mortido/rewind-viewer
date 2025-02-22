// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type Triangle struct {
	_tab flatbuffers.Table
}

func GetRootAsTriangle(buf []byte, offset flatbuffers.UOffsetT) *Triangle {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &Triangle{}
	x.Init(buf, n+offset)
	return x
}

func FinishTriangleBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsTriangle(buf []byte, offset flatbuffers.UOffsetT) *Triangle {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &Triangle{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedTriangleBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *Triangle) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *Triangle) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *Triangle) Color(obj *Color) *Color {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		x := rcv._tab.Indirect(o + rcv._tab.Pos)
		if obj == nil {
			obj = new(Color)
		}
		obj.Init(rcv._tab.Bytes, x)
		return obj
	}
	return nil
}

func (rcv *Triangle) Points(obj *Vector2f, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 8
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *Triangle) PointsLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func TriangleStart(builder *flatbuffers.Builder) {
	builder.StartObject(2)
}
func TriangleAddColor(builder *flatbuffers.Builder, color flatbuffers.UOffsetT) {
	builder.PrependUOffsetTSlot(0, flatbuffers.UOffsetT(color), 0)
}
func TriangleAddPoints(builder *flatbuffers.Builder, points flatbuffers.UOffsetT) {
	builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(points), 0)
}
func TriangleStartPointsVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT {
	return builder.StartVector(8, numElems, 4)
}
func TriangleEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

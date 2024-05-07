// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type Polyline struct {
	_tab flatbuffers.Table
}

func GetRootAsPolyline(buf []byte, offset flatbuffers.UOffsetT) *Polyline {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &Polyline{}
	x.Init(buf, n+offset)
	return x
}

func FinishPolylineBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsPolyline(buf []byte, offset flatbuffers.UOffsetT) *Polyline {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &Polyline{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedPolylineBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *Polyline) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *Polyline) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *Polyline) Color(obj *Color) *Color {
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

func (rcv *Polyline) Points(obj *Vector2f, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 8
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *Polyline) PointsLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func PolylineStart(builder *flatbuffers.Builder) {
	builder.StartObject(2)
}
func PolylineAddColor(builder *flatbuffers.Builder, color flatbuffers.UOffsetT) {
	builder.PrependUOffsetTSlot(0, flatbuffers.UOffsetT(color), 0)
}
func PolylineAddPoints(builder *flatbuffers.Builder, points flatbuffers.UOffsetT) {
	builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(points), 0)
}
func PolylineStartPointsVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT {
	return builder.StartVector(8, numElems, 4)
}
func PolylineEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

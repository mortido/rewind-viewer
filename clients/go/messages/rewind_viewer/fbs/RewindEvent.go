// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type RewindEvent struct {
	_tab flatbuffers.Table
}

func GetRootAsRewindEvent(buf []byte, offset flatbuffers.UOffsetT) *RewindEvent {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &RewindEvent{}
	x.Init(buf, n+offset)
	return x
}

func FinishRewindEventBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsRewindEvent(buf []byte, offset flatbuffers.UOffsetT) *RewindEvent {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &RewindEvent{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedRewindEventBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *RewindEvent) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *RewindEvent) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *RewindEvent) Key() int8 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetInt8(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *RewindEvent) MutateKey(n int8) bool {
	return rcv._tab.MutateInt8Slot(4, n)
}

func (rcv *RewindEvent) MousePaths(obj *MousePath, j int) bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		x := rcv._tab.Vector(o)
		x += flatbuffers.UOffsetT(j) * 4
		x = rcv._tab.Indirect(x)
		obj.Init(rcv._tab.Bytes, x)
		return true
	}
	return false
}

func (rcv *RewindEvent) MousePathsLength() int {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.VectorLen(o)
	}
	return 0
}

func RewindEventStart(builder *flatbuffers.Builder) {
	builder.StartObject(2)
}
func RewindEventAddKey(builder *flatbuffers.Builder, key int8) {
	builder.PrependInt8Slot(0, key, 0)
}
func RewindEventAddMousePaths(builder *flatbuffers.Builder, mousePaths flatbuffers.UOffsetT) {
	builder.PrependUOffsetTSlot(1, flatbuffers.UOffsetT(mousePaths), 0)
}
func RewindEventStartMousePathsVector(builder *flatbuffers.Builder, numElems int) flatbuffers.UOffsetT {
	return builder.StartVector(4, numElems, 4)
}
func RewindEventEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

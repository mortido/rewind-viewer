// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type EndFrame struct {
	_tab flatbuffers.Table
}

func GetRootAsEndFrame(buf []byte, offset flatbuffers.UOffsetT) *EndFrame {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &EndFrame{}
	x.Init(buf, n+offset)
	return x
}

func FinishEndFrameBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsEndFrame(buf []byte, offset flatbuffers.UOffsetT) *EndFrame {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &EndFrame{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedEndFrameBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *EndFrame) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *EndFrame) Table() flatbuffers.Table {
	return rcv._tab
}

func EndFrameStart(builder *flatbuffers.Builder) {
	builder.StartObject(0)
}
func EndFrameEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

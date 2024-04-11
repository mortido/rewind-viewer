// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
)

type ColorT struct {
	Value uint32 `json:"value"`
	Fill bool `json:"fill"`
}

func (t *ColorT) Pack(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	if t == nil {
		return 0
	}
	ColorStart(builder)
	ColorAddValue(builder, t.Value)
	ColorAddFill(builder, t.Fill)
	return ColorEnd(builder)
}

func (rcv *Color) UnPackTo(t *ColorT) {
	t.Value = rcv.Value()
	t.Fill = rcv.Fill()
}

func (rcv *Color) UnPack() *ColorT {
	if rcv == nil {
		return nil
	}
	t := &ColorT{}
	rcv.UnPackTo(t)
	return t
}

type Color struct {
	_tab flatbuffers.Table
}

func GetRootAsColor(buf []byte, offset flatbuffers.UOffsetT) *Color {
	n := flatbuffers.GetUOffsetT(buf[offset:])
	x := &Color{}
	x.Init(buf, n+offset)
	return x
}

func FinishColorBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.Finish(offset)
}

func GetSizePrefixedRootAsColor(buf []byte, offset flatbuffers.UOffsetT) *Color {
	n := flatbuffers.GetUOffsetT(buf[offset+flatbuffers.SizeUint32:])
	x := &Color{}
	x.Init(buf, n+offset+flatbuffers.SizeUint32)
	return x
}

func FinishSizePrefixedColorBuffer(builder *flatbuffers.Builder, offset flatbuffers.UOffsetT) {
	builder.FinishSizePrefixed(offset)
}

func (rcv *Color) Init(buf []byte, i flatbuffers.UOffsetT) {
	rcv._tab.Bytes = buf
	rcv._tab.Pos = i
}

func (rcv *Color) Table() flatbuffers.Table {
	return rcv._tab
}

func (rcv *Color) Value() uint32 {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(4))
	if o != 0 {
		return rcv._tab.GetUint32(o + rcv._tab.Pos)
	}
	return 0
}

func (rcv *Color) MutateValue(n uint32) bool {
	return rcv._tab.MutateUint32Slot(4, n)
}

func (rcv *Color) Fill() bool {
	o := flatbuffers.UOffsetT(rcv._tab.Offset(6))
	if o != 0 {
		return rcv._tab.GetBool(o + rcv._tab.Pos)
	}
	return false
}

func (rcv *Color) MutateFill(n bool) bool {
	return rcv._tab.MutateBoolSlot(6, n)
}

func ColorStart(builder *flatbuffers.Builder) {
	builder.StartObject(2)
}
func ColorAddValue(builder *flatbuffers.Builder, value uint32) {
	builder.PrependUint32Slot(0, value, 0)
}
func ColorAddFill(builder *flatbuffers.Builder, fill bool) {
	builder.PrependBoolSlot(1, fill, false)
}
func ColorEnd(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	return builder.EndObject()
}

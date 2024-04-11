// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import (
	flatbuffers "github.com/google/flatbuffers/go"
	"strconv"
)

type Command byte

const (
	CommandNONE       Command = 0
	CommandCircle     Command = 1
	CommandRectangle  Command = 2
	CommandTriangle   Command = 3
	CommandPolyline   Command = 4
	CommandLogText    Command = 5
	CommandPopup      Command = 6
	CommandPopupRound Command = 7
	CommandOptions    Command = 8
	CommandEndFrame   Command = 9
)

var EnumNamesCommand = map[Command]string{
	CommandNONE:       "NONE",
	CommandCircle:     "Circle",
	CommandRectangle:  "Rectangle",
	CommandTriangle:   "Triangle",
	CommandPolyline:   "Polyline",
	CommandLogText:    "LogText",
	CommandPopup:      "Popup",
	CommandPopupRound: "PopupRound",
	CommandOptions:    "Options",
	CommandEndFrame:   "EndFrame",
}

var EnumValuesCommand = map[string]Command{
	"NONE":       CommandNONE,
	"Circle":     CommandCircle,
	"Rectangle":  CommandRectangle,
	"Triangle":   CommandTriangle,
	"Polyline":   CommandPolyline,
	"LogText":    CommandLogText,
	"Popup":      CommandPopup,
	"PopupRound": CommandPopupRound,
	"Options":    CommandOptions,
	"EndFrame":   CommandEndFrame,
}

func (v Command) String() string {
	if s, ok := EnumNamesCommand[v]; ok {
		return s
	}
	return "Command(" + strconv.FormatInt(int64(v), 10) + ")"
}

type CommandT struct {
	Type Command
	Value interface{}
}

func (t *CommandT) Pack(builder *flatbuffers.Builder) flatbuffers.UOffsetT {
	if t == nil {
		return 0
	}
	switch t.Type {
	case CommandCircle:
		return t.Value.(*CircleT).Pack(builder)
	case CommandRectangle:
		return t.Value.(*RectangleT).Pack(builder)
	case CommandTriangle:
		return t.Value.(*TriangleT).Pack(builder)
	case CommandPolyline:
		return t.Value.(*PolylineT).Pack(builder)
	case CommandLogText:
		return t.Value.(*LogTextT).Pack(builder)
	case CommandPopup:
		return t.Value.(*PopupT).Pack(builder)
	case CommandPopupRound:
		return t.Value.(*PopupRoundT).Pack(builder)
	case CommandOptions:
		return t.Value.(*OptionsT).Pack(builder)
	case CommandEndFrame:
		return t.Value.(*EndFrameT).Pack(builder)
	}
	return 0
}

func (rcv Command) UnPack(table flatbuffers.Table) *CommandT {
	switch rcv {
	case CommandCircle:
		var x Circle
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandCircle, Value: x.UnPack()}
	case CommandRectangle:
		var x Rectangle
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandRectangle, Value: x.UnPack()}
	case CommandTriangle:
		var x Triangle
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandTriangle, Value: x.UnPack()}
	case CommandPolyline:
		var x Polyline
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandPolyline, Value: x.UnPack()}
	case CommandLogText:
		var x LogText
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandLogText, Value: x.UnPack()}
	case CommandPopup:
		var x Popup
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandPopup, Value: x.UnPack()}
	case CommandPopupRound:
		var x PopupRound
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandPopupRound, Value: x.UnPack()}
	case CommandOptions:
		var x Options
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandOptions, Value: x.UnPack()}
	case CommandEndFrame:
		var x EndFrame
		x.Init(table.Bytes, table.Pos)
		return &CommandT{Type: CommandEndFrame, Value: x.UnPack()}
	}
	return nil
}

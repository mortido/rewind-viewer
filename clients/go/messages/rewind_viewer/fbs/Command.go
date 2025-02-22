// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import "strconv"

type Command byte

const (
	CommandNONE          Command = 0
	CommandSubscribe     Command = 1
	CommandUnsubscribe   Command = 2
	CommandCreateAction  Command = 3
	CommandRemoveAction  Command = 4
	CommandReadEvents    Command = 5
	CommandLayer         Command = 6
	CommandMap           Command = 7
	CommandEndFrame      Command = 8
	CommandArc           Command = 9
	CommandCircle        Command = 10
	CommandCircleSegment Command = 11
	CommandPolyline      Command = 12
	CommandRectangle     Command = 13
	CommandTiles         Command = 14
	CommandTriangle      Command = 15
	CommandStartProto    Command = 16
	CommandEndProto      Command = 17
	CommandDrawProto     Command = 18
	CommandLogText       Command = 19
	CommandPopup         Command = 20
	CommandPopupRound    Command = 21
	CommandCameraView    Command = 22
	CommandText          Command = 23
)

var EnumNamesCommand = map[Command]string{
	CommandNONE:          "NONE",
	CommandSubscribe:     "Subscribe",
	CommandUnsubscribe:   "Unsubscribe",
	CommandCreateAction:  "CreateAction",
	CommandRemoveAction:  "RemoveAction",
	CommandReadEvents:    "ReadEvents",
	CommandLayer:         "Layer",
	CommandMap:           "Map",
	CommandEndFrame:      "EndFrame",
	CommandArc:           "Arc",
	CommandCircle:        "Circle",
	CommandCircleSegment: "CircleSegment",
	CommandPolyline:      "Polyline",
	CommandRectangle:     "Rectangle",
	CommandTiles:         "Tiles",
	CommandTriangle:      "Triangle",
	CommandStartProto:    "StartProto",
	CommandEndProto:      "EndProto",
	CommandDrawProto:     "DrawProto",
	CommandLogText:       "LogText",
	CommandPopup:         "Popup",
	CommandPopupRound:    "PopupRound",
	CommandCameraView:    "CameraView",
	CommandText:          "Text",
}

var EnumValuesCommand = map[string]Command{
	"NONE":          CommandNONE,
	"Subscribe":     CommandSubscribe,
	"Unsubscribe":   CommandUnsubscribe,
	"CreateAction":  CommandCreateAction,
	"RemoveAction":  CommandRemoveAction,
	"ReadEvents":    CommandReadEvents,
	"Layer":         CommandLayer,
	"Map":           CommandMap,
	"EndFrame":      CommandEndFrame,
	"Arc":           CommandArc,
	"Circle":        CommandCircle,
	"CircleSegment": CommandCircleSegment,
	"Polyline":      CommandPolyline,
	"Rectangle":     CommandRectangle,
	"Tiles":         CommandTiles,
	"Triangle":      CommandTriangle,
	"StartProto":    CommandStartProto,
	"EndProto":      CommandEndProto,
	"DrawProto":     CommandDrawProto,
	"LogText":       CommandLogText,
	"Popup":         CommandPopup,
	"PopupRound":    CommandPopupRound,
	"CameraView":    CommandCameraView,
	"Text":          CommandText,
}

func (v Command) String() string {
	if s, ok := EnumNamesCommand[v]; ok {
		return s
	}
	return "Command(" + strconv.FormatInt(int64(v), 10) + ")"
}

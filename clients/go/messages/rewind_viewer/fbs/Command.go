// Code generated by the FlatBuffers compiler. DO NOT EDIT.

package fbs

import "strconv"

type Command byte

const (
	CommandNONE          Command = 0
	CommandArc           Command = 1
	CommandCameraView    Command = 2
	CommandCircle        Command = 3
	CommandCircleSegment Command = 4
	CommandCreateAction  Command = 5
	CommandLogText       Command = 6
	CommandOptions       Command = 7
	CommandPolyline      Command = 8
	CommandPopup         Command = 9
	CommandPopupRound    Command = 10
	CommandReadEvents    Command = 11
	CommandRectangle     Command = 12
	CommandRemoveAction  Command = 13
	CommandSubscribe     Command = 14
	CommandTiles         Command = 15
	CommandTriangle      Command = 16
	CommandUnsubscribe   Command = 17
	CommandEndFrame      Command = 18
)

var EnumNamesCommand = map[Command]string{
	CommandNONE:          "NONE",
	CommandArc:           "Arc",
	CommandCameraView:    "CameraView",
	CommandCircle:        "Circle",
	CommandCircleSegment: "CircleSegment",
	CommandCreateAction:  "CreateAction",
	CommandLogText:       "LogText",
	CommandOptions:       "Options",
	CommandPolyline:      "Polyline",
	CommandPopup:         "Popup",
	CommandPopupRound:    "PopupRound",
	CommandReadEvents:    "ReadEvents",
	CommandRectangle:     "Rectangle",
	CommandRemoveAction:  "RemoveAction",
	CommandSubscribe:     "Subscribe",
	CommandTiles:         "Tiles",
	CommandTriangle:      "Triangle",
	CommandUnsubscribe:   "Unsubscribe",
	CommandEndFrame:      "EndFrame",
}

var EnumValuesCommand = map[string]Command{
	"NONE":          CommandNONE,
	"Arc":           CommandArc,
	"CameraView":    CommandCameraView,
	"Circle":        CommandCircle,
	"CircleSegment": CommandCircleSegment,
	"CreateAction":  CommandCreateAction,
	"LogText":       CommandLogText,
	"Options":       CommandOptions,
	"Polyline":      CommandPolyline,
	"Popup":         CommandPopup,
	"PopupRound":    CommandPopupRound,
	"ReadEvents":    CommandReadEvents,
	"Rectangle":     CommandRectangle,
	"RemoveAction":  CommandRemoveAction,
	"Subscribe":     CommandSubscribe,
	"Tiles":         CommandTiles,
	"Triangle":      CommandTriangle,
	"Unsubscribe":   CommandUnsubscribe,
	"EndFrame":      CommandEndFrame,
}

func (v Command) String() string {
	if s, ok := EnumNamesCommand[v]; ok {
		return s
	}
	return "Command(" + strconv.FormatInt(int64(v), 10) + ")"
}

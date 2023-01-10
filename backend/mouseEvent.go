package main

/*
#cgo LDFLAGS:-L./core -Wl,-rpath=./core -lcore -lstdc++
#cgo CFLAGS: -I./core
#include "bridge.h"
#include <stdlib.h>

void goFuncForCallback(char*);
*/
import "C"

type MouseEvent struct {
	Event int `json:"event"`
	X     int `json:"x"`
	Y     int `json:"y"`
}

type MouseEventInfo struct {
	MoveEvent              MouseEvent
	LeftButtonPressEvent   MouseEvent
	LeftButtonReleaseEvent MouseEvent
}

var LastMouseEventInfo MouseEventInfo
var PreviousMouseEventInfo MouseEventInfo

func IsSamePosition(pre MouseEvent, now MouseEvent) bool {
	return (pre.X == now.X && pre.Y == now.Y)
}

func ProcessMouseMoveEvenet() int {
	if LastMouseEventInfo.MoveEvent.X == -1 {
		return -1
	}
	res := int(C.onMouseEventC(C.int(LastMouseEventInfo.MoveEvent.Event), C.int(LastMouseEventInfo.MoveEvent.X), C.int(LastMouseEventInfo.MoveEvent.Y)))
	LastMouseEventInfo.MoveEvent.X = -1
	NeedRender = true
	return res
}

func ProcessMouseLeftButtonPressEvent() int {
	if LastMouseEventInfo.LeftButtonPressEvent.X == -1 {
		return -1
	}
	res := int(C.onMouseEventC(C.int(LastMouseEventInfo.LeftButtonPressEvent.Event), C.int(LastMouseEventInfo.LeftButtonPressEvent.X), C.int(LastMouseEventInfo.LeftButtonPressEvent.Y)))
	C.onMouseEventC(C.int(0), C.int(LastMouseEventInfo.LeftButtonPressEvent.X), C.int(LastMouseEventInfo.LeftButtonPressEvent.Y))
	LastMouseEventInfo.LeftButtonPressEvent.X = -1
	NeedRender = true
	return res
}

func ProcessMouseLeftButtonReleaseEvent() int {
	if LastMouseEventInfo.LeftButtonReleaseEvent.X == -1 {
		return -1
	}
	C.onMouseEventC(C.int(0), C.int(LastMouseEventInfo.LeftButtonReleaseEvent.X), C.int(LastMouseEventInfo.LeftButtonReleaseEvent.Y))
	res := int(C.onMouseEventC(C.int(LastMouseEventInfo.LeftButtonReleaseEvent.Event), C.int(LastMouseEventInfo.LeftButtonReleaseEvent.X), C.int(LastMouseEventInfo.LeftButtonReleaseEvent.Y)))
	LastMouseEventInfo.LeftButtonReleaseEvent.X = -1
	NeedRender = true
	return res
}

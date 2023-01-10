package main

/*
#cgo LDFLAGS:-L./lib -Wl,-rpath=./lib -lcore -lstdc++
#cgo CFLAGS: -I./core
#include "bridge.h"
#include <stdlib.h>

void goFuncForCallback(char*);
*/
import "C"
import (
	"encoding/json"
	"log"
	"time"

	"github.com/gin-contrib/static"
	"github.com/gin-gonic/gin"
	"github.com/gorilla/websocket"
)

func OnMouseEvenet(data string) {
	var event MouseEvent
	err := json.Unmarshal([]byte(data), &event)
	if err != nil {
		log.Println("mouse event format error")
		return
	}
	switch event.Event {
	case 0:
		LastMouseEventInfo.MoveEvent = event
		break
	case 1:
		LastMouseEventInfo.LeftButtonPressEvent = event
		break
	case 2:
		LastMouseEventInfo.LeftButtonReleaseEvent = event
		break
	}

}

var imgBase64 = []byte{}
var width = 700
var height = 700

// 按指定频率将渲染结果通过ws发送至每一个客户端
func SendRenderResult() {
	ticker := time.NewTicker(5 * time.Millisecond)
	defer ticker.Stop()

	for {
		<-ticker.C
		data := imgBase64
		SocketManager.ForEach(func(id string, ws *websocket.Conn) {
			ws.WriteMessage(1, data)
		})
	}

}

func ProcessRenderEvent() {
	var base64ImgC *C.char = C.RenderToStringC()
	imgBase64 = []byte(C.GoString(base64ImgC))
	// C.free(unsafe.Pointer(base64ImgC))
}

var NeedRender = true

// 事件处理循环
func StillProcess() {
	//延迟启动事件处理循环
	timer1 := time.NewTimer(1 * time.Second)
	go func(t *time.Timer) {
		<-t.C
		for true {
			ProcessMouseMoveEvenet()
			ProcessMouseLeftButtonPressEvent()
			ProcessMouseLeftButtonReleaseEvent()
			if !NeedRender {
				continue
			}
			go log.Println("render")
			ProcessRenderEvent()
			NeedRender = false
		}
	}(timer1)

}

func main() {

	go C.runC(C.callbackFuncProto(C.goFuncForCallback), C.int(width), C.int(height))

	go StillProcess()
	go SendRenderResult()

	r := gin.Default()

	// 路由
	r.GET("/ws", SocketManager.Upgrade)

	//静态页面
	r.Use(static.Serve("/", static.LocalFile("wwwroot", false)))

	r.Run("127.0.0.1:8000")
}

var send = true

//export goFuncForCallback
func goFuncForCallback(base64Img *C.char) {
	// 用于c回调go函数(暂时废弃)
	imgBase64 = []byte(C.GoString(base64Img))
	return
}

package main

import (
	"log"
	"net/http"
	"strings"
	"sync"

	"github.com/gin-gonic/gin"
	"github.com/google/uuid"
	"github.com/gorilla/websocket"
)

var upGrader = websocket.Upgrader{
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

type SocketMapStruct struct {
	conns map[string]*websocket.Conn
	lock  sync.Mutex
}

var SocketManager = SocketMapStruct{make(map[string]*websocket.Conn), sync.Mutex{}}

func (s *SocketMapStruct) AddConn(key string, ws *websocket.Conn) {
	s.lock.Lock()
	defer s.lock.Unlock()
	s.conns[key] = ws
}

func (s *SocketMapStruct) DestoryConn(key string) {
	s.lock.Lock()
	defer s.lock.Unlock()
	ws, ok := s.conns[key]
	if !ok {
		return
	}
	ws.Close()
	delete(s.conns, key)
}

func (s *SocketMapStruct) ForEach(function func(key string, value *websocket.Conn)) {
	s.lock.Lock()
	defer s.lock.Unlock()
	for key, v := range s.conns {
		function(key, v)
	}
}

func (s *SocketMapStruct) Upgrade(c *gin.Context) {
	// 升级get请求为webSocket协议
	ws, err := upGrader.Upgrade(c.Writer, c.Request, nil)
	if err != nil {
		return
	}
	uuid := uuid.New()
	key := uuid.String()
	s.AddConn(key, ws)
	defer s.DestoryConn(key)

	for {
		// 读取ws中的数据
		mt, message, err := ws.ReadMessage()
		if err != nil {
			log.Panicln("ws ReadMessage error")
			break
		}
		if mt == 1 {
			var msg []string = strings.Split(string(message), ".")
			switch msg[0] {
			case "MouseEvent":
				OnMouseEvenet(msg[1])
			}
		}
	}
}

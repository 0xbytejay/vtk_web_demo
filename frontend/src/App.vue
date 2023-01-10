<script setup lang="ts">
import { onBeforeUnmount, onBeforeUpdate, onMounted, ref } from "vue"
import { MouseEventInfo } from "./assets/script/ts/type";

let vtk_view = ref<HTMLCanvasElement>(null as unknown as HTMLCanvasElement)

let ctx: CanvasRenderingContext2D
let interval:number

onMounted(() => {
  ctx = vtk_view.value!.getContext("2d") as CanvasRenderingContext2D
  var imgData = ctx.createImageData(700, 700);
  ctx.putImageData(imgData, 0, 0);
  setTimer(200)
})

const setTimer=(wait:number)=>{
  clearInterval(interval)
  interval = setInterval(()=>{
    console.log("checkWebsocket")
    if (websocket===undefined || websocket.readyState===websocket.CLOSED){
      initWebSocket()
      return
    }
  },wait)
}


let websocket:WebSocket

const initWebSocket = () => {
  console.log("initWebSocket")
  let url = "ws://"+window.location.host+"/ws"
  websocket = new WebSocket(url);
  websocket.binaryType = "arraybuffer";
  let buffer = new ArrayBuffer(16);
  let view = new Uint8Array(buffer);
  view[0] = 123
  websocket.onopen = () => {
    clearInterval(interval)
    // 发送数据
    console.log("websocket发送数据中");
  };

  // 客户端接收服务端返回的数据
  websocket.onmessage = evt => {
    let img:HTMLImageElement = new Image();
    img.src = "data:image/bmp;base64," + evt.data;
    ctx.drawImage(img,0,0,1000,1000)
  };
  // 发生错误时
  websocket.onerror = evt => {
    setTimer(200)
    console.log("websocket错误：", evt);
  };
  // 关闭连接
  websocket.onclose = evt => {
    setTimer(200)
    console.log("websocket关闭：", evt);
  };
}





//canvas事件

let r = 700.0 / 500.0
let mouseEventMsg:MouseEventInfo
let MousePressInterval:number
let StillClick=()=>{
  websocket.send("MouseEvent"+"."+JSON.stringify({
        event:1,
        x:mouseEventMsg.x ,
        y:mouseEventMsg.y
}))
}

//将canvas click事件转化为vtk press事件
const canvasMouseEvent=(event:number,e:MouseEvent)=>{
        mouseEventMsg={
        event:event,
        x:Math.floor(e.offsetX * r) ,
        y:Math.floor((500 - e.offsetY) * r)
      }
    if (event === 1){
      clearInterval(MousePressInterval)
      MousePressInterval = setInterval(()=>{
      StillClick()
    },10)
    }else if (event === 2){
      clearInterval(MousePressInterval)
    }
    websocket.send("MouseEvent"+"."+JSON.stringify(mouseEventMsg))
}


const canvasMousewheelEvent = (e:WheelEvent)=>{
  console.log(e)
}

</script>

<template>
  <div>
    <div>
      <canvas id="vtk_view"  v-on:mouseout="canvasMouseEvent(2,$event)" v-on:mouseup="canvasMouseEvent(2,$event)" v-on:mousedown="canvasMouseEvent(1,$event)"  v-on:mousemove="canvasMouseEvent(0,$event)"  ref="vtk_view" width="1000" height="1000"  style="width:500px; height:500px;border:1px solid red;cursor: pointer;"> </canvas>
    </div>
  </div>
</template>

<style scoped>
.logo {
  height: 6em;
  padding: 1.5em;
  will-change: filter;
}

.logo:hover {
  filter: drop-shadow(0 0 2em #646cffaa);
}

.logo.vue:hover {
  filter: drop-shadow(0 0 2em #42b883aa);
}

#vtk_view{
  filter: contrast(130%) brightness(105%);
}
</style>

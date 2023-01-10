#include "core.h"
#include "utils.h"
#include "bridge.h"
#include "stdio.h"



void runC(callbackFuncProto goFunc,int width,int heigh) {
    run(goFunc, width, heigh);
}

int onMouseEventC(int event,int x,int y){
  return onMouseEvent(event,x,y);
}

char* RenderToStringC(){
  return RenderToString();
}

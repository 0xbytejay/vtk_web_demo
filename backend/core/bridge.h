#ifdef __cplusplus
//golang->pure_c_bridge->c++
extern "C" {
#endif
    //回调函数，需与go中定义函数形式一致
    typedef void(*callbackFuncProto) (char* base64Img);

    //接收C的函数指针，用于被C回调
    void runC(callbackFuncProto func,int width,int heigh);
    //鼠标事件处理
    int onMouseEventC(int event,int x,int y);
    //将vtk渲染窗口图像转为base64字符串
    char* RenderToStringC();

#ifdef __cplusplus
}
#endif
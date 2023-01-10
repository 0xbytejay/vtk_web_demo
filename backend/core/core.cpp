#include <stdio.h>
#include <iostream>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkXRenderWindowInteractor.h>
#include <array>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleUser.h>
#include <vtkObjectFactory.h>
#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkBase64Utilities.h>
#include <bridge.h>
#include <vtkCMLMoleculeReader.h>
#include <vtkMoleculeMapper.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkInteractorStyleSwitch.h>
#include <vtkConeSource.h>
#include <X11/Xlib.h>
#include "core.h"
#include "utils.h"

callbackFuncProto callbackFunc;
vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
vtkSmartPointer<vtkWindowToImageFilter> windowToImage = vtkSmartPointer<vtkWindowToImageFilter>::New();
vtkNew<vtkPNGWriter> imageWriter;
vtkSmartPointer<vtkInteractorStyleSwitch> interactorStyle = vtkSmartPointer<vtkInteractorStyleSwitch>::New();
vtkNew<vtkNamedColors> colors;
vtkNew<vtkRenderer> render;



char *RenderToString()
{
  try
  {
    /* code */
    renderWindowInteractor->ProcessEvents();
    windowToImage->Modified();
    imageWriter->Write();
    vtkUnsignedCharArray *result = imageWriter->GetResult();
    char *base64Img = GetBase64EncodedImage(result);
    return base64Img;
  }
  catch (const std::exception &e)
  {
    return NULL;
  }
}

// 处理鼠标事件
int onMouseEvent(int event, int x, int y)
{
  renderWindowInteractor->SetEventPosition(x, y);
  switch (event)
  {
  case 0:
    /* code */
    renderWindowInteractor->MouseMoveEvent();
    break;
  case 1:
    /* code */
      /* code */
    renderWindowInteractor->LeftButtonPressEvent();
    break;
  case 2:
    /* code */
    renderWindowInteractor->LeftButtonReleaseEvent();
    break;
  default:
    break;
  }
  return EXIT_SUCCESS;
}

int startRenderView(int width,int height)
{
  
  renderWindow->AddRenderer(render);
  renderWindow->SetWindowName("VTK Golang Web Demo");

  renderWindowInteractor->SetInteractorStyle(interactorStyle);
  // interactorStyle->SetCurrentStyleToTrackballCamera();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Initialize();
  renderWindow->SetSize(width, height);
  renderWindow->Render();
  // 显示坐标系的vtk组件
  vtkNew<vtkAxesActor> axes_actor;
  axes_actor->SetPosition(0, 0, 0);
  axes_actor->SetTotalLength(2, 2, 2);
  axes_actor->SetShaftType(0);
  axes_actor->SetCylinderRadius(0.02);

  // 控制坐标系，使之随视角共同变化
  vtkNew<vtkOrientationMarkerWidget> widget;
  widget->SetOrientationMarker(axes_actor);
  widget->SetInteractor(renderWindowInteractor);
  widget->SetEnabled(1);
  widget->InteractiveOn();

  // TODO: We should add logic to check if a new rendering needs to be done and
  // then alone do a new rendering otherwise use the cached image.
  windowToImage->SetInput(renderWindow);
  windowToImage->SetInputBufferTypeToRGB();
  windowToImage->ReadFrontBufferOff(); // read from the back buffer
  windowToImage->ShouldRerenderOff();
  windowToImage->FixBoundaryOn();
  windowToImage->Update();

  // imageWriter->SetFileName("/home/jaywang/Desktop/dev/vtk/CylinderExample/build/out.png");

  imageWriter->SetInputConnection(windowToImage->GetOutputPort());
  imageWriter->WriteToMemoryOn();
  renderWindowInteractor->ProcessEvents();
  RenderToString();
  renderWindow->SetShowWindow(true);
  while (true);
  return EXIT_SUCCESS;
}

int ReadCml()
{
  char *a = "porphyrin.cml";
  std::string fname(a);
  vtkNew<vtkCMLMoleculeReader> cmlSource;
  cmlSource->SetFileName(fname.c_str());
  vtkNew<vtkMoleculeMapper> molmapper;
  molmapper->SetInputConnection(cmlSource->GetOutputPort());
  molmapper->UseBallAndStickSettings();
  vtkNew<vtkNamedColors> colors;
  vtkNew<vtkActor> actor;
  actor->SetMapper(molmapper);
  actor->GetProperty()->SetDiffuse(0.7);
  actor->GetProperty()->SetSpecular(0.5);
  actor->GetProperty()->SetSpecularPower(20.0);
  render->AddActor(actor);
  // // Finally render the scene
  // renderWindow->SetMultiSamples(1);
  return EXIT_SUCCESS;
}

int Cone()
{
  // Create a cone
  vtkNew<vtkConeSource> coneSource;
  coneSource->Update();

  // Create a mapper and actor
  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(coneSource->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  // actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("bisque").GetData());
  // Add the actors to the scene
  render->AddActor(actor);
  // render->SetBackground(colors->GetColor3d("Salmon").GetData());
  return EXIT_SUCCESS;
}

int run(callbackFuncProto goFunc, int width, int height)
{
  XInitThreads();
  try
  {
    /* code */
    callbackFunc = goFunc;
    // ReadCml();
    Cone();
    startRenderView(width,height);
  }
  catch (const std::exception &e)
  {
    std::cerr << "runError" << e.what() << '\n';
  }
  return EXIT_SUCCESS;
}



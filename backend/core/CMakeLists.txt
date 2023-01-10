cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(core)

set(VTK_DIR /home/jaywang/Desktop/VTK-9.2.2/VTK-9.2.2/build)


find_package(VTK COMPONENTS
  CommonColor
  CommonCore
  FiltersSources
  InteractionStyle
  vtkInteractionImage
  vtkIOExport
  IOChemistry
  CommonDataModel
  DomainsChemistry
  DomainsChemistryOpenGL2
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  CommonTransforms
  RenderingAnnotation
  vtkInteractionWidgets
)



if (NOT VTK_FOUND)
  message(FATAL_ERROR "Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
# add_executable(CylinderExample MACOSX_BUNDLE CylinderExample.cxx )
#   target_link_libraries(CylinderExample PRIVATE ${VTK_LIBRARIES}
# )

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../lib")


include_directories(${PROJECT_SOURCE_DIR})

add_library(core SHARED core.cpp bridge.cpp utils.cpp)
target_link_libraries(core PRIVATE ${VTK_LIBRARIES} X11
)



# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS core
  MODULES ${VTK_LIBRARIES}
)

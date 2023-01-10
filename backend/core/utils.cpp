#include "utils.h"
#include <vtkNew.h>
#include <vtkUnsignedCharArray.h>
#include <vtkBase64Utilities.h>

//func GetBase64EncodedImage from vtkDataEncoder.cxx
//Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
vtkNew<vtkUnsignedCharArray> LastBase64Image;
char* GetBase64EncodedImage(vtkUnsignedCharArray* encodedInputImage)
  {
    LastBase64Image->SetNumberOfComponents(1);
    LastBase64Image->SetNumberOfTuples(
      std::ceil(1.5 * encodedInputImage->GetNumberOfTuples()));
    unsigned long size = vtkBase64Utilities::Encode(encodedInputImage->GetPointer(0),
      encodedInputImage->GetNumberOfTuples(), LastBase64Image->GetPointer(0), /*mark_end=*/0);

    LastBase64Image->SetNumberOfTuples(static_cast<vtkIdType>(size) + 1);
    LastBase64Image->SetValue(size, 0);
    return reinterpret_cast<char*>(LastBase64Image->GetPointer(0));
  }
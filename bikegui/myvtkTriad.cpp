
#include <cstdlib>
#include <string>
#include <vector>


#include <QtGui>
#include <QVTKWidget.h>
// vtk sources
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkArrowSource.h>
#include <vtkPlaneSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
// vtk filters
#include <vtkTransformPolyDataFilter.h>
// vtk mappers
#include <vtkTextMapper.h>
#include <vtkPolyDataMapper.h>
// vtk actors
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAssembly.h>
// vtk misc
#include <vtkTransform.h>
#include <vtkImageViewer.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
//#include <vtkJPEGReader.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include "parameters.h"
#include "mainwindow.h"
#include "myqwhipple.h"

myvtkTriad::myvtkTriad(vtkSmartPointer<vtkRenderer> renderer)
{
  triadSources.resize(3);
  triadMappers.resize(3);
  triadActors.resize(3);
  for (int i = 0; i < 3; i++) {
    triadSources[i] = vtkArrowSource::New();
    triadSources[i]->SetTipResolution(100);
    triadSources[i]->SetShaftResolution(100);
    triadSources[i]->Update();
    triadMappers[i] = vtkPolyDataMapper::New();
    triadMappers[i]->SetInputConnection(triadSources[i]->GetOutputPort());
    triadActors[i] = vtkActor::New();
    triadActors[i]->SetMapper(triadMappers[i]);
    renderer->AddActor(triadActors[i]);
  }
      
 // arrow1->SetTipRadius(1);
 // arrow1->SetShaftRadius(2);

  triadActors[0]->GetProperty()->SetColor(1.0,0.0,0.0);
  triadActors[1]->GetProperty()->SetColor(0.0,1.0,0.0);
  triadActors[2]->GetProperty()->SetColor(0.0,0.0,1.0);
  triadActors[1]->RotateZ(90);
  triadActors[2]->RotateY(-90);
}

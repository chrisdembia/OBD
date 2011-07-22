#ifndef MYVTKTRIAD_H
#define MYVTKTRIAD_H
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


class myvtkTriad
{
  public:
  myvtkTriad(vtkSmartPointer<vtkRenderer>);
  void AddOrientation(double,double,double);
  void SetOrientation(double,double,double);
  void AddPosition(double,double,double);
  void SetPosition(double,double,double);
  void AddOrientation(double v[3]);
  void SetOrientation(double v[3]);
  void AddPosition(double v[3]);
  void SetPosition(double v[3]);
  void SetScale(double,double,double);
  private:
  // unit vector arrows
  std::vector< vtkSmartPointer<vtkArrowSource> > triadSources;
  std::vector< vtkSmartPointer<vtkPolyDataMapper> > triadMappers;
  std::vector< vtkSmartPointer<vtkActor> > triadActors;
};

#endif

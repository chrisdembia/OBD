#ifndef MYVTKTRIAD_H
#define MYVTKTRIAD_H
#include <cstdlib>
#include <string>
#include <vector>
// vtk sources
#include <vtkSmartPointer.h>
#include <vtkArrowSource.h>
// vtk filters
#include <vtkTransformPolyDataFilter.h>
// vtk mappers
#include <vtkPolyDataMapper.h>
// vtk actors
#include <vtkActor.h>
#include <vtkAssembly.h>
// vtk misc
#include <vtkMath.h>
#include <vtkTransform.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCommand.h>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
// obdgui

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
  void SetRelScale(int dir, double s);
  void SetColor(double,double,double);
  void SetOpacity(double);
  private:
  // unit vector arrows
  std::vector< vtkSmartPointer<vtkArrowSource> > triadSources;
  std::vector< vtkSmartPointer<vtkPolyDataMapper> > triadMappers;
  std::vector< vtkSmartPointer<vtkActor> > triadActors;
  double scalex;
  double scaley;
  double scalez;
};

#endif

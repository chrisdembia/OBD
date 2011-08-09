#ifndef MYVTKTRIAD_H
#define MYVTKTRIAD_H

// c
#include <cstdlib>
#include <vector>
#include <algorithm>

// vtk
#include <vtkSmartPointer.h>
class vtkActor;
class vtkArrowSource;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkPolyDataMapper;
class vtkProperty;

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
  void SetVisibility(double);
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

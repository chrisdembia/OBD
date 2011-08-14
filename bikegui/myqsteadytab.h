#ifndef MYQSTEADYTAB_H
#define MYQSTEADYTAB_H

// c
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>
class QVTKWidget;

// vtk
#include <vtkSmartPointer.h>
class vtkImplicitFunction;
class vtkSampleFunction;
class vtkContourFilter;
class vtkOutlineFilter;
class vtkPolyDataMapper;
class vtkCubeAxesActor;
class vtkCommand;
class vtkAxesActor;
class vtkActor;
class vtkProperty;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;

// gui
class MyQWhipple;
class myvtkHolonomic;
class myvtkTriad;

class MyQSteadyTab : public QWidget {
  Q_OBJECT;

  public:
  MyQSteadyTab(std::vector<MyQWhipple*>* qb,QWidget *parent = 0);


  private:
  std::vector<MyQWhipple*>* qbikes;

  QGridLayout *steadyLayout;
  QVTKWidget *steadyQVTKW;


  vtkSmartPointer<myvtkHolonomic> holoFunc;

  // Sample the function
  vtkSmartPointer<vtkSampleFunction> sample;

  // Create the 0 isosurface
  vtkSmartPointer<vtkContourFilter> contours;

  // Map the contours to graphical primitives
  vtkSmartPointer<vtkPolyDataMapper> contourMapper;

  // Create an actor for the contours
  vtkSmartPointer<vtkActor> contourActor;

  // -- create a box around the function to indicate the sampling volume --

  // Create outline
  vtkSmartPointer<vtkOutlineFilter> outline;

  // Map it to graphics primitives
  vtkSmartPointer<vtkPolyDataMapper> outlineMapper;

  // Create an actor for it
  vtkSmartPointer<vtkActor> outlineActor;

  // Visualize
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkRenderWindow> renderWindow;
  vtkSmartPointer<vtkRenderWindowInteractor> interactor;

};
#endif

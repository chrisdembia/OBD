#ifndef MYQSTEADYTAB_H
#define MYQSTEADYTAB_H

#include <cstdlib>
#include <string>
#include <vector>
#include <QtGui>
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>

#include <vtkSphere.h>
#include <vtkImplicitFunction.h>
#include <vtkObjectFactory.h>
#include <cmath>

#include "myqwhipple.h"
#include "myvtkHolonomic.h"


class MyQSteadyTab : public QWidget
{
  Q_OBJECT;

  public:
  MyQSteadyTab(std::vector<MyQWhipple*>* qb,QWidget *parent = 0);


  private:
  std::vector<MyQWhipple*>* qbikes;

  QGridLayout* steadyLayout;
  QVTKWidget* steadyQVTKW;


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

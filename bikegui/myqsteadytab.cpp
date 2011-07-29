
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
#include <vtkCubeAxesActor.h>
#include <vtkAxesActor.h>
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
#include "myqsteadytab.h"
#include "myvtkHolonomic.h"
#include "myvtkTriad.h"

MyQSteadyTab::MyQSteadyTab(std::vector<MyQWhipple*>* qb, QWidget *parent) :
  QWidget(parent)
{

  qbikes = qb;

  steadyLayout = new QGridLayout(this);
  steadyQVTKW = new QVTKWidget(this);
  steadyLayout->addWidget(steadyQVTKW);

  holoFunc = vtkSmartPointer<myvtkHolonomic>::New();
  holoFunc->SetBike(qbikes->at(0)->getBike());

  // Sample the function
  sample = vtkSmartPointer<vtkSampleFunction>::New();
  sample->SetSampleDimensions(100,100,100);
  sample->SetImplicitFunction(holoFunc);
  sample->SetModelBounds(-M_PI/2.0, M_PI/2.0,
      M_PI/10.-M_PI, M_PI/10.+M_PI,
      0.0, M_PI);
 
  // Create the 0 isosurface
  contours = vtkSmartPointer<vtkContourFilter>::New();
  contours->SetInput(sample->GetOutput());
  contours->GenerateValues(1, 0, 0);
 
  // Map the contours to graphical primitives
  contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  contourMapper->SetInput(contours->GetOutput());
//  contourMapper->SetScalarRange(0.0, 1.2);
 
  // Create an actor for the contours
  contourActor = vtkSmartPointer<vtkActor>::New();
  contourActor->SetMapper(contourMapper);
//  contourActor->GetProperty()->SetColor(0,0,0);
 
  // -- create a box around the function to indicate the sampling volume --
 
  // Create outline
  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outline->SetInput(sample->GetOutput());
 
  // Map it to graphics primitives
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineMapper->SetInput(outline->GetOutput());
 
  // Create an actor for it
  outlineActor = vtkSmartPointer<vtkActor>::New();
  outlineActor->SetMapper(outlineMapper);
  outlineActor->GetProperty()->SetColor(0,0,0);
 
  // Visualize
  renderer = vtkSmartPointer<vtkRenderer>::New();
  renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
//  interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  interactor->SetRenderWindow(renderWindow);
 
  steadyQVTKW->SetRenderWindow(renderWindow);
//  myvtkTriad triad1(renderer);
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  renderer->AddActor(axes);
  vtkSmartPointer<vtkCubeAxesActor> axes1 = vtkSmartPointer<vtkCubeAxesActor>::New();
  axes1->SetCamera(renderer->GetActiveCamera());
  axes1->SetXTitle("lean");
  axes1->SetYTitle("pitch");
  axes1->SetZTitle("steer");
  axes1->SetBounds(sample->GetModelBounds());
  renderer->AddActor(axes1);
/*
  vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
  vtkSmartPointer<vtkPolyDataMapper> spheremap = vtkSmartPointer<vtkPolyDataMapper>::New();
  spheremap->SetInput(sphere->GetOutput());
  vtkSmartPointer<vtkActor> sphereact = vtkSmartPointer<vtkActor>::New();
  sphereact->SetMapper(spheremap);
  renderer->AddActor(sphereact);*/

  renderer->AddActor(contourActor);
  renderer->AddActor(outlineActor);
  renderer->SetBackground(.5,.5,.5); // Background color white
 
  renderWindow->SetAAFrames(2);
  renderWindow->Render();
  renderer->ResetCamera();

//  interactor->Start();
  steadyQVTKW->GetInteractor()->Start();
 
}


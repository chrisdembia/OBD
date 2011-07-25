// cstd
#include <cstdlib>
#include <vector>
// qt
// vtk sources
#include <vtkSmartPointer.h>
#include <vtkArrowSource.h>
// vtk filters
#include <vtkTransformPolyDataFilter.h>
// vtk mappers
#include <vtkTextMapper.h>
#include <vtkPolyDataMapper.h>
// vtk actors
#include <vtkActor.h>
#include <vtkAssembly.h>
// vtk misc
#include <vtkTransform.h>
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

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
// obdgui
#include "myvtkTriad.h"

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


void myvtkTriad::SetOrientation(double vx, double vy, double vz)
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->SetOrientation(vx,vy,vz);
  }
  triadActors[1]->AddOrientation(0,0,90);
  triadActors[2]->AddOrientation(0,-90,0);
}

void myvtkTriad::SetOrientation(double v[3])
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->SetOrientation(v[0],v[1],v[2]);
  }
  triadActors[1]->RotateZ(90);
  triadActors[2]->RotateY(-90);
//  triadActors[1]->AddOrientation(0,0,90);
//  triadActors[2]->AddOrientation(0,-90,0);
}

void myvtkTriad::AddOrientation(double vx, double vy, double vz)
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->AddOrientation(vx,vy,vz);
  }
}

void myvtkTriad::AddOrientation(double v[3])
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->AddOrientation(v[0],v[1],v[2]);
  }
}

void myvtkTriad::SetPosition(double px, double py, double pz)
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->SetPosition(px,py,pz);
  }
}

void myvtkTriad::SetPosition(double p[3])
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->SetPosition(p[0],p[1],p[2]);
  }
}

void myvtkTriad::AddPosition(double px, double py, double pz)
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->AddPosition(px,py,pz);
  }
}

void myvtkTriad::AddPosition(double p[3])
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->AddPosition(p[0],p[1],p[2]);
  }
}

void myvtkTriad::SetScale(double sx,double sy,double sz)
{
  for (int i = 0; i < 3; i++) {
    triadActors[i]->SetScale(sx,sy,sz);
  }
}

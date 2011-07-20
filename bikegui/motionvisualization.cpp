
#include <cstdlib>
#include <string>
#include <vector>

#include <ctime>

#include <QtGui>
#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkArrowSource.h>
#include <vtkPlaneSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAssembly.h>
#include <vtkImageViewer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkJPEGReader.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
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

void MainWindow::simulateSlot(void)
{
  // WHIPPLE CODE
  bike->evalConstants();
  bike->eoms();
  bike->computeOutputs();

  std::string outfolder = "";
  std::string filename = outfolder; filename += "simulation.dat";
  std::ofstream OutputFile(filename.c_str(), std::ios::binary);

  OutputFile << bike;
  double tj, state[10] = {bike->q0, bike->q1, bike->q3, bike->q4, bike->q5,
                          bike->q6, bike->q7, bike->u1, bike->u3, bike->u5};

  for (int j = 1; j < bike->fps*bike->tf + 1; ++j) {
    tj = ((double) j) / ((double) bike->fps);
    while (bike->t < tj)
      bike->evolve(tj, state);
    bike->computeOutputs();
    OutputFile << bike;
  } // for j

  OutputFile.close();
  std::cout << "Simulation completed. Simulation output written to "
    << outfolder << '\n';
/*
  cyl->SetResolution(8);

  vtkPolyDataMapper *cylMapper = vtkPolyDataMapper::New();
  cylMapper->SetInputConnection(cyl->GetOutputPort());

  vtkActor *cylActor = vtkActor::New();
  cylActor->SetMapper(cylMapper);
  cylActor->GetProperty()->SetColor(1.000,0.3882,0.2784);
  cylActor->RotateX(30.0);
  cylActor->RotateY(-45.0);



  // MAKE HUD-DISPLAY, TEXTACTOR:w
  VTK_CREATE(vtkTextProperty,text1Propr);
  text1Propr->SetFontSize(10);
  text1Propr->SetJustificationToCentered();
  VTK_CREATE(vtkTextMapper,text1Mapper);
  VTK_CREATE(vtkActor2D,text1Actor2D);
    // CAN I LUMP TOGETHER DIFFERENT "SOURCES" INTO ONE ACTOR?"
  motionRenderer->SetBackground(1,1,1);
  motionRenderer->ResetCamera();
*/
  // draw coordinate triad
  myvtkTriad triad1(motionRenderer);
  // ground
  VTK_CREATE(vtkPlaneSource,groundSource);
  groundSource->SetNormal(0,0,-1);
  groundSource->Update();
  VTK_CREATE(vtkPolyDataMapper,groundMapper);
  groundMapper->SetInputConnection(groundSource->GetOutputPort());
  VTK_CREATE(vtkActor,groundActor);
  groundActor->SetMapper(groundMapper);
  groundActor->GetProperty()->SetOpacity(0.5);
  // draw a bike
  // rear wheel: two cones and a torus
  double rearwheelcenter[3] = {0,0,-bike->rr-bike->rrt};
  double hubhalfwidth = .1;
  // rear cone right
  // source
  VTK_CREATE(vtkConeSource,rearConeRightSource);
  rearConeRightSource->SetHeight(hubhalfwidth);
  rearConeRightSource->SetRadius(bike->rr);
  rearConeRightSource->SetResolution(100);
  rearConeRightSource->Update();
  // transform
  VTK_CREATE(vtkTransform,rearConeRightTransform);
  rearConeRightTransform->Identity();
  rearConeRightTransform->Translate(0,hubhalfwidth/2,0);
  rearConeRightTransform->RotateZ(90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,rearConeRightFilter);
  rearConeRightFilter->SetInputConnection(rearConeRightSource->GetOutputPort());
  rearConeRightFilter->SetTransform(
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearConeRightMapper);
  rearConeRightMapper->SetInputConnection(rearConeRightFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearConeRightActor);
  rearConeRightActor->SetMapper(rearConeRightMapper);
  rearConeRightActor->GetProperty()->SetOpacity(0.3);
  // rear cone left
  // source
  VTK_CREATE(vtkConeSource,rearConeLeftSource);
  rearConeLeftSource->SetHeight(hubhalfwidth);
  rearConeLeftSource->SetRadius(bike->rr);
  rearConeLeftSource->SetResolution(100);
  rearConeLeftSource->Update();
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearConeLeftMapper);
  rearConeLeftMapper->SetInputConnection(rearConeLeftSource->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearConeLeftActor);
  rearConeLeftActor->SetMapper(rearConeLeftMapper);
  rearConeLeftActor->SetOrientation(0,0,-90);
 // rearConeLeftActor->AddPosition(0,-hubhalfwidth/2,rearwheelcenter[2]);
  rearConeLeftActor->AddPosition(0,-hubhalfwidth/2,0);
  rearConeLeftActor->GetProperty()->SetOpacity(0.3);
  // rear torus
  VTK_CREATE(vtkParametricTorus,rearTorus);
  // source
  VTK_CREATE(vtkParametricFunctionSource,rearTorusSource);
  rearTorusSource->SetParametricFunction(rearTorus);
  rearTorus->SetRingRadius(bike->rr);
  rearTorus->SetCrossSectionRadius(bike->rrt+.02);
  rearTorusSource->Update();
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearTorusMapper);
  rearTorusMapper->SetInputConnection(rearTorusSource->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearTorusActor);
  rearTorusActor->SetMapper(rearTorusMapper);
  rearTorusActor->SetOrientation(90,0,0);
//  rearTorusActor->AddPosition(rearwheelcenter);

  // rear wheel assembly
  VTK_CREATE(vtkAssembly,rearWheelAssy);
  rearWheelAssy->AddPart(rearConeRightActor);
  rearWheelAssy->AddPart(rearConeLeftActor);
  rearWheelAssy->AddPart(rearTorusActor);
  myvtkTriad triad4(motionRenderer);
  triad4.SetPosition(rearWheelAssy->GetPosition());
  triad4.SetOrientation(rearWheelAssy->GetOrientation());
  triad4.SetScale(.2,.2,.2);
/*  
  // rear frame mass center ball
  VTK_CREATE(vtkSphereSource,rearCOMSource);
  rearCOMSource->SetRadius(0.08);
  rearCOMSource->Update();
  VTK_CREATE(vtkPolyDataMapper,rearCOMMapper);
  rearCOMMapper->SetInputConnection(rearCOMSource->GetOutputPort());
  VTK_CREATE(vtkActor,rearCOMActor);
  rearCOMActor->SetMapper(rearCOMMapper);
  rearCOMActor->SetOrigin(-bike->lrx,0,-bike->lrz);
  rearCOMActor->AddPosition(bike->lrx,0,bike->lrz+rearwheelcenter[2]);
  rearCOMActor->RotateY(180/M_PI*bike->q2);
  myvtkTriad triad3(motionRenderer);
  triad3.SetPosition(rearCOMActor->GetPosition());
  triad3.SetOrientation(rearCOMActor->GetOrientation());
  triad3.SetScale(.2,.2,.2);
  // MUST MOVE CENTER OF ROTATION TO WHEEL HUB, AND THEN ROTATE BY PITCH ANGLE
  // rear frame tube
  VTK_CREATE(vtkCylinderSource,rearTubeSource);
  rearTubeSource->SetRadius(0.012);
  rearTubeSource->SetHeight(bike->lr);
  VTK_CREATE(vtkPolyDataMapper,rearTubeMapper);
  rearTubeMapper->SetInputConnection(rearTubeSource->GetOutputPort());
  VTK_CREATE(vtkActor,rearTubeActor);
  rearTubeActor->SetMapper(rearTubeMapper);
  rearTubeActor->SetOrigin(0,bike->lr/2,0);
  rearTubeActor->SetPosition(0,-bike->lr/2,rearwheelcenter[2]);
  rearTubeActor->RotateZ(90);
  rearTubeActor->RotateX(180/M_PI*bike->q2);
  myvtkTriad triad2(motionRenderer);
  triad2.SetPosition(rearTubeActor->GetPosition());
  triad2.SetOrientation(rearTubeActor->GetOrientation());
  triad2.SetScale(.2,.2,.2);
*/
//  motionRenderer->AddActor(groundActor);
//  motionRenderer->AddActor(rearConeRightActor);
 // motionRenderer->AddActor(rearConeLeftActor);
 // motionRenderer->AddActor(rearTorusActor);
 // motionRenderer->AddActor(rearCOMActor);
 // motionRenderer->AddActor(rearTubeActor);
 //
  // render the actor assemblies
  motionRenderer->AddActor(rearWheelAssy);
  motionRenderer->SetBackground(.8,1,.8);
  motionQVTKW->GetInteractor()->Initialize();
  
//  motionRenderWindow->SetAAFrames(2);
  // camera settings
  motionRenderer->ResetCamera();
  motionRenderer->GetActiveCamera()->SetPosition(0,1,0);
 // motionRenderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  motionRenderer->GetActiveCamera()->Elevation(-95);
  QTime simtime;
  simtime.start();
  int framectr = 0;
  int frameswitch = 0;
  int period = 1000/30; //bike->fps;
 // cylActor->RotateX(.01*framectr);
  while (simtime.elapsed() < 1)
  {
    if ( simtime.elapsed() % period < 1 && frameswitch == 0)
    {
      framectr++;
      frameswitch = 1;
      motionRenderer->ResetCamera();
 //     rearTorusActor->AddPosition(.01,0,0);
//  motionRenderer->GetActiveCamera()->Elevation(.01*framectr);
  //    rearTorusActor->
//      motionRenderer->Render();
      motionQVTKW->GetRenderWindow()->Render();
      std::cout << simtime.elapsed() << " fn " << framectr << std::endl;
    }
    if (simtime.elapsed() % period > 1)
    {
      frameswitch = 0;
    }

}
}

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
      
//  arrow1->SetTipLength(10);
 // arrow1->SetTipRadius(1);
 // arrow1->SetShaftRadius(2);

  triadActors[0]->GetProperty()->SetColor(1.0,0.0,0.0);
  triadActors[1]->GetProperty()->SetColor(0.0,1.0,0.0);
  triadActors[2]->GetProperty()->SetColor(0.0,0.0,1.0);
  triadActors[1]->RotateZ(90);
  triadActors[2]->RotateY(-90);
  triadSources[0]->Print(std::cout);
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
/*
void MainWindow::drawTriad(void)
{
  // unit vector arrows
  std::vector< vtkSmartPointer<vtkArrowSource> > triadSources(3);
  std::vector< vtkSmartPointer<vtkPolyDataMapper> > triadMappers(3);
  std::vector< vtkSmartPointer<vtkActor> > triadActors(3);
  for (int i = 0; i < 3; i++) {
    triadSources[i] = vtkArrowSource::New();
    triadSources[i]->SetTipResolution(100);
    triadSources[i]->SetShaftResolution(100);
    triadSources[i]->Update();
    triadMappers[i] = vtkPolyDataMapper::New();
    triadMappers[i]->SetInputConnection(triadSources[i]->GetOutputPort());
    triadActors[i] = vtkActor::New();
    triadActors[i]->SetMapper(triadMappers[i]);
    motionRenderer->AddActor(triadActors[i]);
  }
      
//  arrow1->SetTipLength(10);
 // arrow1->SetTipRadius(1);
 // arrow1->SetShaftRadius(2);

  triadActors[0]->GetProperty()->SetColor(1.0,0.0,0.0);
  triadActors[1]->GetProperty()->SetColor(0.0,1.0,0.0);
  triadActors[2]->GetProperty()->SetColor(0.0,0.0,1.0);
  triadActors[1]->RotateZ(90);
  triadActors[2]->RotateY(-90);
  triadSources[0]->Print(std::cout);

  
}
*/

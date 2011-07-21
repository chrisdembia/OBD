
#include <cstdlib>
#include <string>
#include <vector>

#include <ctime>

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
#include <vtkPolyDataMapper.h>
// vtk actors
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAssembly.h>
// vtk misc
#include <vtkTransform.h>
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

class myvtkBike
{
  public:
    myvtkBike(vtkSmartPointer<vtkRenderer>);
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
  myvtkTriad triad0(motionRenderer);
  // ground
  VTK_CREATE(vtkPlaneSource,groundSource);
  groundSource->SetNormal(0,0,-1);
  groundSource->Update();
  VTK_CREATE(vtkPolyDataMapper,groundMapper);
  groundMapper->SetInputConnection(groundSource->GetOutputPort());
  VTK_CREATE(vtkActor,groundActor);
  groundActor->SetMapper(groundMapper);
  groundActor->SetScale(3,3,3);
  groundActor->GetProperty()->SetOpacity(0.5);
  // draw a bike
  // rear wheel: two cones and a torus
  double rearwheelcenter[3] = {0,0, -bike->rr - bike->rrt};
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
  rearConeRightFilter->SetTransform(rearConeRightTransform);
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
  // transform
  VTK_CREATE(vtkTransform,rearConeLeftTransform);
  rearConeLeftTransform->Identity();
  rearConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  rearConeLeftTransform->RotateZ(-90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,rearConeLeftFilter);
  rearConeLeftFilter->SetInputConnection(rearConeLeftSource->GetOutputPort());
  rearConeLeftFilter->SetTransform(rearConeLeftTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearConeLeftMapper);
  rearConeLeftMapper->SetInputConnection(rearConeLeftFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearConeLeftActor);
  rearConeLeftActor->SetMapper(rearConeLeftMapper);
  rearConeLeftActor->GetProperty()->SetOpacity(0.3);
  
  // rear torus
  VTK_CREATE(vtkParametricTorus,rearTorus);
  // source
  VTK_CREATE(vtkParametricFunctionSource,rearTorusSource);
  rearTorusSource->SetParametricFunction(rearTorus);
  rearTorus->SetRingRadius(bike->rr);
  rearTorus->SetCrossSectionRadius(bike->rrt+.02);
  rearTorusSource->Update();
  // transform
  VTK_CREATE(vtkTransform,rearTorusTransform);
  rearTorusTransform->Identity();
  rearTorusTransform->RotateX(90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,rearTorusFilter);
  rearTorusFilter->SetInputConnection(rearTorusSource->GetOutputPort());
  rearTorusFilter->SetTransform(rearTorusTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearTorusMapper);
  rearTorusMapper->SetInputConnection(rearTorusFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearTorusActor);
  rearTorusActor->SetMapper(rearTorusMapper);

  // rear wheel assembly
  VTK_CREATE(vtkAssembly,rearWheelAssy);
  rearWheelAssy->AddPart(rearConeRightActor);
  rearWheelAssy->AddPart(rearConeLeftActor);
  rearWheelAssy->AddPart(rearTorusActor);
  rearWheelAssy->SetPosition(rearwheelcenter);
  myvtkTriad triad1(motionRenderer);
  triad1.SetPosition(rearWheelAssy->GetPosition());
  triad1.SetOrientation(rearWheelAssy->GetOrientation());
  triad1.SetScale(.2,.2,.2);

  // rear frame: center of mass ball and tube
  // rear center of mass ball
  // source
  VTK_CREATE(vtkSphereSource,rearCOMSource);
  rearCOMSource->SetRadius(0.08);
  rearCOMSource->SetThetaResolution(100);
  rearCOMSource->SetPhiResolution(100);
  rearCOMSource->Update();
  // transform
  VTK_CREATE(vtkTransform,rearCOMTransform);
  rearCOMTransform->Identity();
  rearCOMTransform->Translate(bike->lrx,0,bike->lrz);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,rearCOMFilter);
  rearCOMFilter->SetInputConnection(rearCOMSource->GetOutputPort());
  rearCOMFilter->SetTransform(rearCOMTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearCOMMapper);
  rearCOMMapper->SetInputConnection(rearCOMFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearCOMActor);
  rearCOMActor->SetMapper(rearCOMMapper);
  
  // rear frame tube
  // source
  VTK_CREATE(vtkCylinderSource,rearTubeSource);
  rearTubeSource->SetRadius(0.012);
  rearTubeSource->SetHeight(bike->lr);
  // transform
  VTK_CREATE(vtkTransform,rearTubeTransform);
  rearTubeTransform->Identity();
  rearTubeTransform->RotateZ(90);
  rearTubeTransform->Translate(0,-bike->lr/2,0);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,rearTubeFilter);
  rearTubeFilter->SetInputConnection(rearTubeSource->GetOutputPort());
  rearTubeFilter->SetTransform(rearTubeTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,rearTubeMapper);
  rearTubeMapper->SetInputConnection(rearTubeFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,rearTubeActor);
  rearTubeActor->SetMapper(rearTubeMapper);

  VTK_CREATE(vtkAssembly,rearFrameAssy);
  rearFrameAssy->AddPart(rearCOMActor);
  rearFrameAssy->AddPart(rearTubeActor);
  rearFrameAssy->SetPosition(rearwheelcenter);
  rearFrameAssy->RotateY(180/M_PI*bike->q2);

  myvtkTriad triad2(motionRenderer);
  triad2.SetPosition(rearFrameAssy->GetPosition());
  triad2.SetOrientation(rearFrameAssy->GetOrientation());
  triad2.SetScale(.2,.2,.2);

  // front frame
  // front frame tube
  // source
  VTK_CREATE(vtkCylinderSource,frontTubeSource);
  frontTubeSource->SetRadius(0.012);
  frontTubeSource->SetHeight(bike->ls);
  // transform
  VTK_CREATE(vtkTransform,frontTubeTransform);
  frontTubeTransform->Identity();
  frontTubeTransform->RotateZ(90);
  frontTubeTransform->Translate(0,-bike->ls/2,0);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,frontTubeFilter);
  frontTubeFilter->SetInputConnection(frontTubeSource->GetOutputPort());
  frontTubeFilter->SetTransform(frontTubeTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,frontTubeMapper);
  frontTubeMapper->SetInputConnection(frontTubeFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,frontTubeActor);
  frontTubeActor->SetMapper(frontTubeMapper);

  // front frame offset
  // source
  VTK_CREATE(vtkCylinderSource,frontOffsetSource);
  frontOffsetSource->SetRadius(0.012);
  frontOffsetSource->SetHeight(bike->lf);
  frontOffsetSource->DebugOn();
  // transform
  VTK_CREATE(vtkTransform,frontOffsetTransform);
  frontOffsetTransform->Identity();
  frontOffsetTransform->RotateX(90);
  frontOffsetTransform->Translate(bike->ls,-bike->lf/2,0);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,frontOffsetFilter);
  frontOffsetFilter->SetInputConnection(frontOffsetSource->GetOutputPort());
  frontOffsetFilter->SetTransform(frontOffsetTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,frontOffsetMapper);
  frontOffsetMapper->SetInputConnection(frontOffsetFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,frontOffsetActor);
  frontOffsetActor->SetMapper(frontOffsetMapper);

  VTK_CREATE(vtkAssembly,frontFrameAssy);
  frontFrameAssy->AddPart(frontTubeActor);
  frontFrameAssy->AddPart(frontOffsetActor);
  frontFrameAssy->SetPosition(0,0,-1);

  // front frame transform
  VTK_CREATE(vtkTransform,frontFrameTransform);
  frontFrameTransform->SetMatrix(rearFrameAssy->GetMatrix());
  frontFrameTransform->Translate(bike->lr,0,0);
  frontFrameTransform->RotateY(-90);
//  frontFrameAssy->PokeMatrix(frontFrameTransform->GetMatrix());
  frontFrameAssy->SetPosition(frontFrameTransform->GetPosition());
  frontFrameAssy->SetOrientation(frontFrameTransform->GetOrientation());

  myvtkTriad triad3(motionRenderer);
  triad3.SetPosition(frontFrameAssy->GetPosition());
  triad3.SetOrientation(frontFrameAssy->GetOrientation());
  triad3.SetScale(.2,.2,.2);

  // front wheel: two cones and a torus
  double frontwheelcenter[3] = {0,0, -bike->rf - bike->rft};
  // front cone right
  // source
  VTK_CREATE(vtkConeSource,frontConeRightSource);
  frontConeRightSource->SetHeight(hubhalfwidth);
  frontConeRightSource->SetRadius(bike->rr);
  frontConeRightSource->SetResolution(100);
  frontConeRightSource->Update();
  // transform
  VTK_CREATE(vtkTransform,frontConeRightTransform);
  frontConeRightTransform->Identity();
  frontConeRightTransform->Translate(0,hubhalfwidth/2,0);
  frontConeRightTransform->RotateZ(90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,frontConeRightFilter);
  frontConeRightFilter->SetInputConnection(frontConeRightSource->GetOutputPort());
  frontConeRightFilter->SetTransform(frontConeRightTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,frontConeRightMapper);
  frontConeRightMapper->SetInputConnection(frontConeRightFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,frontConeRightActor);
  frontConeRightActor->SetMapper(frontConeRightMapper);
  frontConeRightActor->GetProperty()->SetOpacity(0.3);

  // front cone left
  // source
  VTK_CREATE(vtkConeSource,frontConeLeftSource);
  frontConeLeftSource->SetHeight(hubhalfwidth);
  frontConeLeftSource->SetRadius(bike->rr);
  frontConeLeftSource->SetResolution(100);
  frontConeLeftSource->Update();
  // transform
  VTK_CREATE(vtkTransform,frontConeLeftTransform);
  frontConeLeftTransform->Identity();
  frontConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  frontConeLeftTransform->RotateZ(-90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,frontConeLeftFilter);
  frontConeLeftFilter->SetInputConnection(frontConeLeftSource->GetOutputPort());
  frontConeLeftFilter->SetTransform(frontConeLeftTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,frontConeLeftMapper);
  frontConeLeftMapper->SetInputConnection(frontConeLeftFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,frontConeLeftActor);
  frontConeLeftActor->SetMapper(frontConeLeftMapper);
  frontConeLeftActor->GetProperty()->SetOpacity(0.3);
  
  // front torus
  VTK_CREATE(vtkParametricTorus,frontTorus);
  // source
  VTK_CREATE(vtkParametricFunctionSource,frontTorusSource);
  frontTorusSource->SetParametricFunction(frontTorus);
  frontTorus->SetRingRadius(bike->rr);
  frontTorus->SetCrossSectionRadius(bike->rrt+.02);
  frontTorusSource->Update();
  // transform
  VTK_CREATE(vtkTransform,frontTorusTransform);
  frontTorusTransform->Identity();
  frontTorusTransform->RotateX(90);
  // filter
  VTK_CREATE(vtkTransformPolyDataFilter,frontTorusFilter);
  frontTorusFilter->SetInputConnection(frontTorusSource->GetOutputPort());
  frontTorusFilter->SetTransform(frontTorusTransform);
  // mapper
  VTK_CREATE(vtkPolyDataMapper,frontTorusMapper);
  frontTorusMapper->SetInputConnection(frontTorusFilter->GetOutputPort());
  // actor
  VTK_CREATE(vtkActor,frontTorusActor);
  frontTorusActor->SetMapper(frontTorusMapper);

  // front wheel assembly
  VTK_CREATE(vtkAssembly,frontWheelAssy);
  frontWheelAssy->AddPart(frontConeRightActor);
  frontWheelAssy->AddPart(frontConeLeftActor);
  frontWheelAssy->AddPart(frontTorusActor);
  frontWheelAssy->SetPosition(frontwheelcenter);
  
  // front frame transform
  VTK_CREATE(vtkTransform,frontWheelTransform);
  frontWheelTransform->SetMatrix(frontFrameAssy->GetMatrix());
  frontWheelTransform->Translate(bike->ls,0,-bike->lf);
  frontWheelTransform->RotateY(90-180/M_PI*bike->q2);
  frontWheelAssy->SetPosition(frontWheelTransform->GetPosition());
  frontWheelAssy->SetOrientation(frontWheelTransform->GetOrientation());

  myvtkTriad triad4(motionRenderer);
  triad4.SetPosition(frontWheelAssy->GetPosition());
  triad4.SetOrientation(frontWheelAssy->GetOrientation());
  triad4.SetScale(.2,.2,.2);


  // render the actor assemblies
  motionRenderer->AddActor(groundActor);
  motionRenderer->AddActor(rearWheelAssy);
  motionRenderer->AddActor(rearFrameAssy);
  motionRenderer->AddActor(frontFrameAssy);
  motionRenderer->AddActor(frontWheelAssy);
  motionRenderer->SetBackground(.8,1,.8);
  motionQVTKW->GetInteractor()->Initialize();
  
//  motionRenderWindow->SetAAFrames(2);
  // camera settings
  motionRenderer->ResetCamera();
  motionRenderer->GetActiveCamera()->SetPosition(0,1,-.5);
  motionRenderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  motionRenderer->GetActiveCamera()->Elevation(-95);
  QTime simtime;
  simtime.start();
  int framectr = 0;
  int frameswitch = 0;
  int period = 1000/30; //bike->fps;
 // cylActor->RotateX(.01*framectr);
  while (simtime.elapsed() < 10000)
  {
    if ( simtime.elapsed() % period < 1 && frameswitch == 0)
    {
      framectr++;
      frameswitch = 1;
      rearWheelAssy->AddPosition(.01,0,0);
//  motionRenderer->GetActiveCamera()->Elevation(.01*framectr);
  //    rearTorusActor->
//      motionRenderer->Render();
      motionRenderer->ResetCamera();
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

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

class myvtkBike
{
  public:
  myvtkBike(vtkSmartPointer<vtkRenderer>,Whipple*);
  ~myvtkBike() {
    delete triad1;
    delete triad2;
    delete triad3;
    delete triad4;
  }
  void Update();
  Whipple* GetBike() {
    return bike;
  }

  private:
  vtkSmartPointer<vtkRenderer> renderer;
  Whipple* bike;
  // rear wheel: two cones and a torus
  // rear cone right
  // source
  vtkSmartPointer<vtkConeSource> rearConeRightSource;
  // transform
  vtkSmartPointer<vtkTransform> rearConeRightTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> rearConeRightFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> rearConeRightMapper;
  // actor
  vtkSmartPointer<vtkActor> rearConeRightActor;

  // rear cone left
  // source
  vtkSmartPointer<vtkConeSource> rearConeLeftSource;
  // transform
  vtkSmartPointer<vtkTransform> rearConeLeftTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> rearConeLeftFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> rearConeLeftMapper;
  // actor
  vtkSmartPointer<vtkActor> rearConeLeftActor;
  
  // rear torus
  vtkSmartPointer<vtkParametricTorus> rearTorus;
  // source
  vtkSmartPointer<vtkParametricFunctionSource> rearTorusSource;
  // transform
  vtkSmartPointer<vtkTransform> rearTorusTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> rearTorusFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> rearTorusMapper;
  // actor
  vtkSmartPointer<vtkActor> rearTorusActor;

  // rear frame: center of mass ball and tube
  // rear center of mass ball
  // source
  vtkSmartPointer<vtkSphereSource> rearCOMSource;
  // transform
  vtkSmartPointer<vtkTransform> rearCOMTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> rearCOMFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> rearCOMMapper;
  // actor
  vtkSmartPointer<vtkActor> rearCOMActor;
  
  // rear frame tube
  // source
  vtkSmartPointer<vtkCylinderSource> rearTubeSource;
  // transform
  vtkSmartPointer<vtkTransform> rearTubeTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> rearTubeFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> rearTubeMapper;
  // actor
  vtkSmartPointer<vtkActor> rearTubeActor;

  // front frame
  // front frame tube
  // source
  vtkSmartPointer<vtkCylinderSource> frontTubeSource;
  // transform
  vtkSmartPointer<vtkTransform> frontTubeTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> frontTubeFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> frontTubeMapper;
  // actor
  vtkSmartPointer<vtkActor> frontTubeActor;

  // front frame offset
  // source
  vtkSmartPointer<vtkCylinderSource> frontOffsetSource;
  // transform
  vtkSmartPointer<vtkTransform> frontOffsetTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> frontOffsetFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> frontOffsetMapper;
  // actor
  vtkSmartPointer<vtkActor> frontOffsetActor;

  // front wheel: two cones and a torus
  // front cone right
  // source
  vtkSmartPointer<vtkConeSource> frontConeRightSource;
  // transform
  vtkSmartPointer<vtkTransform> frontConeRightTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> frontConeRightFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> frontConeRightMapper;
  // actor
  vtkSmartPointer<vtkActor> frontConeRightActor;

  // front cone left
  // source
  vtkSmartPointer<vtkConeSource> frontConeLeftSource;
  // transform
  vtkSmartPointer<vtkTransform> frontConeLeftTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> frontConeLeftFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> frontConeLeftMapper;
  // actor
  vtkSmartPointer<vtkActor> frontConeLeftActor;
  
  // front torus
  vtkSmartPointer<vtkParametricTorus> frontTorus;
  // source
  vtkSmartPointer<vtkParametricFunctionSource> frontTorusSource;
  // transform
  vtkSmartPointer<vtkTransform> frontTorusTransform;
  // filter
  vtkSmartPointer<vtkTransformPolyDataFilter> frontTorusFilter;
  // mapper
  vtkSmartPointer<vtkPolyDataMapper> frontTorusMapper;
  // actor
  vtkSmartPointer<vtkActor> frontTorusActor;
  
  // vector triads
  myvtkTriad *triad0;
  myvtkTriad *triad1;
  myvtkTriad *triad2;
  myvtkTriad *triad3;
  myvtkTriad *triad4;
  
  // render the actor assemblies
  vtkSmartPointer<vtkAssembly> rearWheelAssy;
  vtkSmartPointer<vtkAssembly> rearFrameAssy;
  vtkSmartPointer<vtkAssembly> frontFrameAssy;
  vtkSmartPointer<vtkTransform> frontFrameTransform;
  vtkSmartPointer<vtkAssembly> frontWheelAssy;
  vtkSmartPointer<vtkTransform> frontWheelTransform;
};

class vtkTimerCallback2 : public vtkCommand
{
  public:
    static vtkTimerCallback2 *New()
    {
      vtkTimerCallback2 *cb = new vtkTimerCallback2;
      cb->TimerCount = 0;
      return cb;
    }
 
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void * vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent == eventId) {
        ++this->TimerCount;
      }
      std::cout << this->TimerCount << std::endl;
      // UPDATE
      time = (double)TimerCount/(double)myvtkbike->GetBike()->fps; 
      while (myvtkbike->GetBike()->t < time) {
        myvtkbike->GetBike()->evolve(time,state);
      }

      myvtkbike->Update();
      // render
      vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
      iren->GetRenderWindow()->Render();
//  w2i->SetInput(iren->GetRenderWindow());
//  writer->SetInput(w2i->GetOutput());
//  writer->SetFileName(QString("cld72qtvtkbike" + QString("%1").arg(TimerCount) + ".ps").toStdString().c_str());
//  writer->Write();
    }
 
  public:
    myvtkBike *myvtkbike;
    void SetState(double s[10]) {
      for (int i = 0; i < 10; i++) {
        state[i] = s[i];
      }
    }
    vtkSmartPointer<vtkPostScriptWriter> writer;
    vtkSmartPointer<vtkWindowToImageFilter> w2i;
  private:
    int TimerCount;
    double time;
    double state[10];
};

void MainWindow::simulateSlot(void)
{
  // WHIPPLE CODE
  bike->evalConstants();
  bike->eoms();
  bike->computeOutputs();
  double state[10] = {bike->q0, bike->q1, bike->q3, bike->q4, bike->q5,
                          bike->q6, bike->q7, bike->u1, bike->u3, bike->u5};
  std::string outfolder = "";
  std::string filename = outfolder; filename += "simulation.dat";
  std::ofstream OutputFile(filename.c_str(), std::ios::binary);
/*
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
*/
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
  groundActor->GetProperty()->SetColor(0,0,0);
  groundActor->GetProperty()->SetOpacity(0.5);
  // draw a bike
  myvtkBike *myvtkbike1 = new myvtkBike(motionRenderer,bike);
  myvtkbike1->Update();
  motionRenderer->SetBackground(.8,1,.8);
  motionQVTKW->GetInteractor()->Initialize();
 
  VTK_CREATE(vtkWindowToImageFilter,w2i);
//  VTK_CREATE(vtkJPEGWriter,writer);
  VTK_CREATE(vtkPostScriptWriter,writer);
//  motionRenderWindow->SetAAFrames(2);
  // camera settings
  motionRenderer->ResetCamera();
  motionRenderer->AddActor(groundActor);
  motionRenderer->GetActiveCamera()->SetPosition(0,1,-.5);
  motionRenderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  motionRenderer->GetActiveCamera()->Elevation(-95);
  // Render and interact
  motionQVTKW->GetRenderWindow()->Render();
 
  // Sign up to receive TimerEvent
  VTK_CREATE(vtkTimerCallback2,callback);
  callback->myvtkbike = myvtkbike1;
  callback->SetState(state);
  callback->writer = writer;
  callback->w2i = w2i;
  motionQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent, callback);
 
  int timerId = motionQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/bike->fps);
  std::cout << "timerId: " << timerId << std::endl;
 
  // Start the interaction and timer
  motionQVTKW->GetInteractor()->Start();
//  delete myvtkbike1;

  /*QTime simtime;
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
 //     rearWheelAssy->AddPosition(.01,0,0);
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
*/
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
  
myvtkBike::myvtkBike(vtkSmartPointer<vtkRenderer> ren,Whipple* b)
{
  renderer = ren;
  bike = b;
  // rear wheel: two cones and a torus
  double hubhalfwidth = .1;
  // rear cone right
  // source
  rearConeRightSource = vtkConeSource::New();
  rearConeRightSource->SetHeight(hubhalfwidth);
  rearConeRightSource->SetRadius(bike->rr);
  rearConeRightSource->SetResolution(100);
  rearConeRightSource->Update();
  // transform
  rearConeRightTransform = vtkTransform::New();
  rearConeRightTransform->Identity();
  rearConeRightTransform->Translate(0,hubhalfwidth/2,0);
  rearConeRightTransform->RotateZ(90);
  // filter
  rearConeRightFilter = vtkTransformPolyDataFilter::New();
  rearConeRightFilter->SetInputConnection(rearConeRightSource->GetOutputPort());
  rearConeRightFilter->SetTransform(rearConeRightTransform);
  // mapper
  rearConeRightMapper = vtkPolyDataMapper::New();
  rearConeRightMapper->SetInputConnection(rearConeRightFilter->GetOutputPort());
  // actor
  rearConeRightActor = vtkActor::New();
  rearConeRightActor->SetMapper(rearConeRightMapper);
  rearConeRightActor->GetProperty()->SetOpacity(0.7);

  // rear cone left
  // source
  rearConeLeftSource = vtkConeSource::New();
  rearConeLeftSource->SetHeight(hubhalfwidth);
  rearConeLeftSource->SetRadius(bike->rr);
  rearConeLeftSource->SetResolution(100);
  rearConeLeftSource->Update();
  // transform
  rearConeLeftTransform = vtkTransform::New();
  rearConeLeftTransform->Identity();
  rearConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  rearConeLeftTransform->RotateZ(-90);
  // filter
  rearConeLeftFilter = vtkTransformPolyDataFilter::New();
  rearConeLeftFilter->SetInputConnection(rearConeLeftSource->GetOutputPort());
  rearConeLeftFilter->SetTransform(rearConeLeftTransform);
  // mapper
  rearConeLeftMapper = vtkPolyDataMapper::New();
  rearConeLeftMapper->SetInputConnection(rearConeLeftFilter->GetOutputPort());
  // actor
  rearConeLeftActor = vtkActor::New();
  rearConeLeftActor->SetMapper(rearConeLeftMapper);
  rearConeLeftActor->GetProperty()->SetOpacity(0.7);
  
  // rear torus
  rearTorus = vtkParametricTorus::New();
  // source
  rearTorusSource = vtkParametricFunctionSource::New();
  rearTorusSource->SetParametricFunction(rearTorus);
  rearTorus->SetRingRadius(bike->rr);
  rearTorus->SetCrossSectionRadius(bike->rrt);
  rearTorusSource->Update();
  // transform
  rearTorusTransform = vtkTransform::New();
  rearTorusTransform->Identity();
  rearTorusTransform->RotateX(90);
  // filter
  rearTorusFilter = vtkTransformPolyDataFilter::New();
  rearTorusFilter->SetInputConnection(rearTorusSource->GetOutputPort());
  rearTorusFilter->SetTransform(rearTorusTransform);
  // mapper
  rearTorusMapper = vtkPolyDataMapper::New();
  rearTorusMapper->SetInputConnection(rearTorusFilter->GetOutputPort());
  // actor
  rearTorusActor = vtkActor::New();
  rearTorusActor->SetMapper(rearTorusMapper);

  // rear frame: center of mass ball and tube
  // rear center of mass ball
  // source
  rearCOMSource = vtkSphereSource::New();
  rearCOMSource->SetRadius(0.08);
  rearCOMSource->SetThetaResolution(100);
  rearCOMSource->SetPhiResolution(100);
  rearCOMSource->Update();
  // transform
  rearCOMTransform = vtkTransform::New();
  rearCOMTransform->Identity();
  rearCOMTransform->Translate(bike->lrx,0,bike->lrz);
  // filter
  rearCOMFilter = vtkTransformPolyDataFilter::New();
  rearCOMFilter->SetInputConnection(rearCOMSource->GetOutputPort());
  rearCOMFilter->SetTransform(rearCOMTransform);
  // mapper
  rearCOMMapper = vtkPolyDataMapper::New();
  rearCOMMapper->SetInputConnection(rearCOMFilter->GetOutputPort());
  // actor
  rearCOMActor = vtkActor::New();
  rearCOMActor->SetMapper(rearCOMMapper);
  
  // rear frame tube
  // source
  rearTubeSource = vtkCylinderSource::New();
  rearTubeSource->SetRadius(0.012);
  rearTubeSource->SetHeight(bike->lr);
  // transform
  rearTubeTransform = vtkTransform::New();
  rearTubeTransform->Identity();
  rearTubeTransform->RotateZ(90);
  rearTubeTransform->Translate(0,-bike->lr/2,0);
  // filter
  rearTubeFilter = vtkTransformPolyDataFilter::New();
  rearTubeFilter->SetInputConnection(rearTubeSource->GetOutputPort());
  rearTubeFilter->SetTransform(rearTubeTransform);
  // mapper
  rearTubeMapper = vtkPolyDataMapper::New();
  rearTubeMapper->SetInputConnection(rearTubeFilter->GetOutputPort());
  // actor
  rearTubeActor = vtkActor::New();
  rearTubeActor->SetMapper(rearTubeMapper);

  // front frame
  // front frame tube
  // source
  frontTubeSource = vtkCylinderSource::New();
  frontTubeSource->SetRadius(0.012);
  frontTubeSource->SetHeight(bike->ls);
  // transform
  frontTubeTransform = vtkTransform::New();
  frontTubeTransform->Identity();
  frontTubeTransform->RotateZ(90);
  frontTubeTransform->Translate(0,-bike->ls/2,0);
  // filter
  frontTubeFilter = vtkTransformPolyDataFilter::New();
  frontTubeFilter->SetInputConnection(frontTubeSource->GetOutputPort());
  frontTubeFilter->SetTransform(frontTubeTransform);
  // mapper
  frontTubeMapper = vtkPolyDataMapper::New();
  frontTubeMapper->SetInputConnection(frontTubeFilter->GetOutputPort());
  // actor
  frontTubeActor = vtkActor::New();
  frontTubeActor->SetMapper(frontTubeMapper);

  // front frame offset
  // source
  frontOffsetSource = vtkCylinderSource::New();
  frontOffsetSource->SetRadius(0.012);
  frontOffsetSource->SetHeight(bike->lf);
//  frontOffsetSource->DebugOn();
  // transform
  frontOffsetTransform = vtkTransform::New();
  frontOffsetTransform->Identity();
  frontOffsetTransform->RotateX(90);
  frontOffsetTransform->Translate(bike->ls,-bike->lf/2,0);
  // filter
  frontOffsetFilter = vtkTransformPolyDataFilter::New();
  frontOffsetFilter->SetInputConnection(frontOffsetSource->GetOutputPort());
  frontOffsetFilter->SetTransform(frontOffsetTransform);
  // mapper
  frontOffsetMapper = vtkPolyDataMapper::New();
  frontOffsetMapper->SetInputConnection(frontOffsetFilter->GetOutputPort());
  // actor
  frontOffsetActor = vtkActor::New();
  frontOffsetActor->SetMapper(frontOffsetMapper);

  // front wheel: two cones and a torus
  // front cone right
  // source
  frontConeRightSource = vtkConeSource::New();
  frontConeRightSource->SetHeight(hubhalfwidth);
  frontConeRightSource->SetRadius(bike->rr);
  frontConeRightSource->SetResolution(100);
  frontConeRightSource->Update();
  // transform
  frontConeRightTransform = vtkTransform::New();
  frontConeRightTransform->Identity();
  frontConeRightTransform->Translate(0,hubhalfwidth/2,0);
  frontConeRightTransform->RotateZ(90);
  // filter
  frontConeRightFilter = vtkTransformPolyDataFilter::New();
  frontConeRightFilter->SetInputConnection(frontConeRightSource->GetOutputPort());
  frontConeRightFilter->SetTransform(frontConeRightTransform);
  // mapper
  frontConeRightMapper = vtkPolyDataMapper::New();
  frontConeRightMapper->SetInputConnection(frontConeRightFilter->GetOutputPort());
  // actor
  frontConeRightActor = vtkActor::New();
  frontConeRightActor->SetMapper(frontConeRightMapper);
  frontConeRightActor->GetProperty()->SetOpacity(0.7);

  // front cone left
  // source
  frontConeLeftSource = vtkConeSource::New();
  frontConeLeftSource->SetHeight(hubhalfwidth);
  frontConeLeftSource->SetRadius(bike->rr);
  frontConeLeftSource->SetResolution(100);
  frontConeLeftSource->Update();
  // transform
  frontConeLeftTransform = vtkTransform::New();
  frontConeLeftTransform->Identity();
  frontConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  frontConeLeftTransform->RotateZ(-90);
  // filter
  frontConeLeftFilter = vtkTransformPolyDataFilter::New();
  frontConeLeftFilter->SetInputConnection(frontConeLeftSource->GetOutputPort());
  frontConeLeftFilter->SetTransform(frontConeLeftTransform);
  // mapper
  frontConeLeftMapper = vtkPolyDataMapper::New();
  frontConeLeftMapper->SetInputConnection(frontConeLeftFilter->GetOutputPort());
  // actor
  frontConeLeftActor = vtkActor::New();
  frontConeLeftActor->SetMapper(frontConeLeftMapper);
  frontConeLeftActor->GetProperty()->SetOpacity(0.7);
  
  // front torus
  frontTorus = vtkParametricTorus::New();
  // source
  frontTorusSource = vtkParametricFunctionSource::New();
  frontTorusSource->SetParametricFunction(frontTorus);
  frontTorus->SetRingRadius(bike->rr);
  frontTorus->SetCrossSectionRadius(bike->rrt);
  frontTorusSource->Update();
  // transform
  frontTorusTransform = vtkTransform::New();
  frontTorusTransform->Identity();
  frontTorusTransform->RotateX(90);
  // filter
  frontTorusFilter = vtkTransformPolyDataFilter::New();
  frontTorusFilter->SetInputConnection(frontTorusSource->GetOutputPort());
  frontTorusFilter->SetTransform(frontTorusTransform);
  // mapper
  frontTorusMapper = vtkPolyDataMapper::New();
  frontTorusMapper->SetInputConnection(frontTorusFilter->GetOutputPort());
  // actor
  frontTorusActor = vtkActor::New();
  frontTorusActor->SetMapper(frontTorusMapper);

  // ASSEMBLIES
  // render the actor assemblies
  // rear wheel assembly
  rearWheelAssy = vtkAssembly::New();
  rearWheelAssy->AddPart(rearConeRightActor);
  rearWheelAssy->AddPart(rearConeLeftActor);
  rearWheelAssy->AddPart(rearTorusActor);

  // rear frame assembly
  rearFrameAssy = vtkAssembly::New();
  rearFrameAssy->AddPart(rearCOMActor);
  rearFrameAssy->AddPart(rearTubeActor);

  // front frame assembly
  frontFrameAssy = vtkAssembly::New();
  frontFrameTransform = vtkTransform::New();
  frontFrameAssy->AddPart(frontTubeActor);
  frontFrameAssy->AddPart(frontOffsetActor);

  // front wheel assembly
  frontWheelAssy = vtkAssembly::New();
  frontWheelTransform = vtkTransform::New();
  frontWheelAssy->AddPart(frontConeRightActor);
  frontWheelAssy->AddPart(frontConeLeftActor);

  // vector coordinate system triads
  triad0 = new myvtkTriad(renderer);
  triad0->SetScale(.4,.4,.4);
  triad1 = new myvtkTriad(renderer);
  triad1->SetScale(.2,.2,.2);
  triad2 = new myvtkTriad(renderer);
  triad2->SetScale(.2,.2,.2);
  triad3 = new myvtkTriad(renderer);
  triad3->SetScale(.2,.2,.2);
  triad4 = new myvtkTriad(renderer);
  triad4->SetScale(.2,.2,.2);

  // render
  renderer->AddActor(rearWheelAssy);
  renderer->AddActor(rearFrameAssy);
  renderer->AddActor(frontFrameAssy);
  renderer->AddActor(frontWheelAssy);
}

void myvtkBike::Update()
{
  double YAW = bike->q0;
  double LEAN = bike->q1;
  double PITCH = bike->q2;
  double STEER = bike->q3;
  double RWSPIN = bike->q4; 
  double FWSPIN = bike->q5;
  double X = bike->q6;
  double Y = bike->q7;
  // ASSEMBLIES
  // rear wheel assembly
  rearWheelAssy->SetPosition(-bike->rr*sin(YAW)*sin(LEAN) + X,
                             bike->rr*cos(YAW)*sin(LEAN) + Y,
                             -bike->rrt - bike->rr*cos(LEAN));
  rearWheelAssy->SetOrientation(0,0,0);
  rearWheelAssy->RotateZ(180/M_PI*YAW);// NEGATIVE? IS RIGHTWARD AS IS
  rearWheelAssy->RotateX(180/M_PI*LEAN);
  rearWheelAssy->RotateY(180/M_PI*RWSPIN);
  
  triad0->SetPosition(X,Y,0);
//  triad0->SetOrientation(rearFrameAssy->GetOrientation());

  triad1->SetPosition(rearWheelAssy->GetPosition());
  triad1->SetOrientation(rearWheelAssy->GetOrientation());

  // rear frame assembly
  rearFrameAssy->SetPosition(rearWheelAssy->GetPosition());
  rearFrameAssy->SetOrientation(rearWheelAssy->GetOrientation());
  rearFrameAssy->RotateY(180/M_PI*(-RWSPIN+PITCH));

  triad2->SetPosition(rearFrameAssy->GetPosition());
  triad2->SetOrientation(rearFrameAssy->GetOrientation());

  // front frame assembly
  frontFrameAssy->SetPosition(0,0,-1);

  // front frame transform
  frontFrameTransform->SetMatrix(rearFrameAssy->GetMatrix());
  frontFrameTransform->Translate(bike->lr,0,0);
  frontFrameTransform->RotateY(-90);
  frontFrameTransform->RotateX(-STEER);
  frontFrameAssy->SetPosition(frontFrameTransform->GetPosition());
  frontFrameAssy->SetOrientation(frontFrameTransform->GetOrientation());

  triad3->SetPosition(frontFrameAssy->GetPosition());
  triad3->SetOrientation(frontFrameAssy->GetOrientation());

  // front wheel assembly
  frontWheelAssy->SetPosition(0,0,-bike->rf - bike->rft);
  
  // front frame transform
  frontWheelTransform->SetMatrix(frontFrameAssy->GetMatrix());
  frontWheelTransform->Translate(bike->ls,0,-bike->lf);
  frontWheelTransform->RotateY(90-180/M_PI*(PITCH-FWSPIN));
  frontWheelAssy->SetPosition(frontWheelTransform->GetPosition());
  frontWheelAssy->SetOrientation(frontWheelTransform->GetOrientation());

  triad4->SetPosition(frontWheelAssy->GetPosition());
  triad4->SetOrientation(frontWheelAssy->GetOrientation());
//      iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
//    renderer->ResetCameraClippingRange();
//
  double d = 1;
  double pos[3];
  pos[0] = frontFrameAssy->GetPosition()[0];
  pos[1] = frontFrameAssy->GetPosition()[1];
  pos[2] = frontFrameAssy->GetPosition()[2];
//  double bounds[6] = frontFrameAssy->GetBounds();
  renderer->ResetCamera(pos[0]-d,pos[0]+d,
      pos[1]-d,pos[1]+d,
      pos[2]-d,pos[2]+d);
//  renderer->GetActiveCamera()->Dolly(.5);

 // renderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
  //frontFrameAssy->AddPosition(0,5,-2);
  //renderer->GetActiveCamera()->SetPosition(frontFrameAssy->GetPosition());
//  renderer->GetActiveCamera()->SetDistance(5);
//  renderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  motionRenderer->GetActiveCamera()->Elevation(-95);
//  renderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
}
 

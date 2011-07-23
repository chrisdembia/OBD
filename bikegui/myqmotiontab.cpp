#include <cstdlib>
#include <string>
#include <vector>
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
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>

#include <vtkWindowToImageFilter.h>
//#include <vtkJPEGReader.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include "parameters.h"
#include "mainwindow.h"
#include "myqwhipple.h"


void myQMotion::createMotionVisualizationTab(void)
{

  QGroupBox *motionLSetBox = new QGroupBox("Parameters",motionVisualizationTab);
  QGridLayout *motionLSetLayout = new QGridLayout(motionLSetBox);
//  QGroupBox* motionRSetBox = new
	QGridLayout *motionLayout = new QGridLayout(motionVisualizationTab);

  motionQVTKW = new QVTKWidget(uprightStabilityTab);
//  motionQVTKW->resize(256,256);
  motionLayout->addWidget(motionLSetBox,0,0);
  motionLayout->addWidget(motionQVTKW,0,1);
  motionVisualizationTab->setLayout(motionLayout);

  // motionLSetBox
  QLabel *label1 = new QLabel("test");
  QToolButton* simulateButton = new QToolButton(motionLSetBox);
  simulateButton->setText("Start simulation");
  connect(simulateButton, SIGNAL(clicked()), this, SLOT(simulateSlot()) );

  // motionLSetBox Widgets to motionLayout
  motionLSetLayout->addWidget(label1);
  motionLSetLayout->addWidget(simulateButton);

// CAN HAVE MULTIPLE RENDERERS IN ONE RENDERWINDOW, DIFF BACKGROUNDS
// AND CAMERA ANGLES
  // DELETE THE POINTERS DELEETE THE POINTERS!:w

  //setup window
  motionRenderWindow = vtkRenderWindow::New();

  //setup renderer
  motionRenderer = vtkRenderer::New();
  motionRenderWindow->AddRenderer(motionRenderer);
  motionQVTKW->SetRenderWindow(motionRenderWindow);
  // USE CMAKE TO IDENTIFY TYPE OF COMPUTER? FOR VIDEO AVI OUTPUT
}

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
      time = (double)TimerCount/(double)qbike->GetBike()->fps; 
      while (qbike->GetBike()->t < time) {
        qbike->GetBike()->evolve(time,state);
      }

      qbike->MotionUpdate();
     // qbike->MotionSetValues(TimerCount);
      // render
      vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
      iren->GetRenderWindow()->Render();
//  w2i->SetInput(iren->GetRenderWindow());
//  writer->SetInput(w2i->GetOutput());
//  writer->SetFileName(QString("cld72qtvtkbike" + QString("%1").arg(TimerCount) + ".ps").toStdString().c_str());
//  writer->Write();
    }
 
  public:
    MyQWhipple *qbike;
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

void myQMotion::simulateSlot(void)
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
  MyQWhipple *qbike1 = new MyQWhipple(motionRenderer,bike);
  qbike1->MotionUpdate();
//  qbike1->MotionSetValues(0);
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
  callback->qbike = qbike1;
  callback->SetState(state);
  callback->writer = writer;
  callback->w2i = w2i;
  motionQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent, callback);
 
  int timerId = motionQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/bike->fps);
  std::cout << "timerId: " << timerId << std::endl;
 
  // Start the interaction and timer
  motionQVTKW->GetInteractor()->Start();
//  delete qbike1;

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



// CAN'T HIT START ANIMATION TWICE!!

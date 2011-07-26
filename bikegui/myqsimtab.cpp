#include <cstdlib>
#include <string>
#include <vector>
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
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>
// vtk plotting
#include <vtkTable.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>

#include <vtkWindowToImageFilter.h>
//#include <vtkJPEGReader.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
#define NMOTIONVARS 32

#include "whipple.h"
#include "myqsimtab.h"
#include "myvtkTriad.h"
#include "myqwhipple.h"


myQSimTab::myQSimTab(std::vector<MyQWhipple*>* qb, Whipple* b, QWidget *parent) : QWidget(parent)
{
  bike = b;
  qbikes = qb;
  simLSetBox = new QGroupBox("Parameters",this);
  simLSetLayout = new QGridLayout(simLSetBox);
//  QGroupBox* simRSetBox = new
	simLayout = new QGridLayout(this);

  simQVTKW = new QVTKWidget(this);
//  simQVTKW->resize(256,256);

  // simLSetBox
  label1 = new QLabel("test");
  startsimButton = new QToolButton(simLSetBox);
  startsimButton->setText("Start simulation");
  connect(startsimButton, SIGNAL(clicked()), this, SLOT(startsimSlot()) );

  stopsimButton = new QToolButton(simLSetBox);
  stopsimButton->setText("Stop simulation");
  connect(stopsimButton, SIGNAL(clicked()), this, SLOT(stopsimSlot()));

// CAN HAVE MULTIPLE RENDERERS IN ONE RENDERWINDOW, DIFF BACKGROUNDS
// AND CAMERA ANGLES
  // DELETE THE POINTERS DELEETE THE POINTERS!:w

  //setup window
  simRenderWindow = vtkRenderWindow::New();
  //setup renderer
  simRenderer = vtkRenderer::New();
  simRenderWindow->AddRenderer(simRenderer);
  simQVTKW->SetRenderWindow(simRenderWindow);
  // USE CMAKE TO IDENTIFY TYPE OF COMPUTER? FOR VIDEO AVI OUTPUT
  qbike1 = new MyQWhipple("bike1",bike);
  qbike1->initSim(simRenderer);

  simCallback = vtkSmartPointer<vtkTimerCallback2>::New();
  simCallback->qbike = qbike1;
  simQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent, simCallback);
//  VTK_CREATE(vtkWindowToImageFilter,w2i);
//  VTK_CREATE(vtkJPEGWriter,writer);
//  VTK_CREATE(vtkPostScriptWriter,writer);
//  simRenderWindow->SetAAFrames(2);
  // Sign up to receive TimerEvent
 // VTK_CREATE(vtkTimerCallback2,callback);

  // plot
  simPlotQVTKW = new QVTKWidget(this);
  simPlotVTKView = vtkSmartPointer<vtkContextView>::New();
  simPlotVTKView->SetInteractor(simPlotQVTKW->GetInteractor());
  simPlotQVTKW->SetRenderWindow(simPlotVTKView->GetRenderWindow());
  simPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  simPlotVTKView->GetScene()->AddItem(simPlotVTKChart);
//  VTK_CREATE(vtkPlot,simPlotVTKLine);
  
  vtkPlot* simPlotVTKLine;
  for (int i = 1; i < NMOTIONVARS; i++) {
    simPlotVTKLine = simPlotVTKChart->AddPlot(vtkChart::LINE);
    simPlotVTKLine->SetInput(qbike1->GetSimTable(), 0, i);
    simPlotVTKLine->SetColor(255,0,0,255);
    // ideally, create 32 vtkPlot simPlotVTKLines

  }

  simCallback->plotrenwin = simPlotQVTKW->GetRenderWindow();
  simCallback->simPlotQVTKW = simPlotQVTKW;
  // arrange layouts
  // simLSetBox Widgets to simLayout
  simLSetLayout->addWidget(label1);
  simLSetLayout->addWidget(startsimButton);
  simLSetLayout->addWidget(stopsimButton);
  
  simLayout->addWidget(simLSetBox,0,0);
  simLayout->addWidget(simQVTKW,0,1);
  simLayout->addWidget(simPlotQVTKW,1,0,1,2);
  setLayout(simLayout);
  
}


void myQSimTab::startsimSlot(void)
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
  simRenderer->SetBackground(1,1,1);
  simRenderer->ResetCamera();
*/
  // draw coordinate triad
  myvtkTriad triad0(simRenderer);
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
  qbike1->MotionUpdate();
//  qbike1->MotionSetValues(0);
  simRenderer->SetBackground(.8,1,.8);
  simQVTKW->GetInteractor()->Initialize();
 
  // camera settings
  simRenderer->ResetCamera();
  simRenderer->AddActor(groundActor);
  simRenderer->GetActiveCamera()->SetPosition(0,1,-.5);
  simRenderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  simRenderer->GetActiveCamera()->Elevation(-95);
  // Render and interact
  simQVTKW->GetRenderWindow()->Render();
 
  simCallback->SetState(state);
//  simCallback->writer = writer;
//  simCallback->w2i = w2i;
 
  int timerId = simQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/bike->fps);
  std::cout << "timerId: " << timerId << std::endl;
 
  // Start the interaction and timer
  simQVTKW->GetInteractor()->Start();
//  delete qbike1;

}

void myQSimTab::stopsimSlot(void)
{
  simQVTKW->GetInteractor()->DestroyTimer();
}

void myQSimTab::updatePlotSlot(void)
{
}





// CAN'T HIT START ANIMATION TWICE!!

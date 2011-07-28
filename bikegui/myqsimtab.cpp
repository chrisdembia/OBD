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


myQSimTab::myQSimTab(std::vector<MyQWhipple*>* qb, QWidget *parent) :
  QWidget(parent)
{
  qbikes = qb;
  simLSetBox = new QGroupBox("Parameters",this);
  simLSetLayout = new QGridLayout(simLSetBox);
//  QGroupBox* simRSetBox = new
	simLayout = new QGridLayout(this);

  simQVTKW = new QVTKWidget(this);
//  simQVTKW->resize(256,256);

  // initial velocity
  speedLabel = new QLabel( tr("initial speed (m/s)"), simLSetBox); 
  speedEdit = new QLineEdit(simLSetBox);
// connect here
  
  // simLSetBox
  startsimButton = new QToolButton(simLSetBox);
  startsimButton->setText( tr("Start simulation") );
  connect(startsimButton, SIGNAL(clicked()), this, SLOT(startsimSlot()) );

  stopsimButton = new QToolButton(simLSetBox);
  stopsimButton->setText( tr("Stop simulation") );
  connect(stopsimButton, SIGNAL(clicked()), this, SLOT(stopsimSlot()));

  // include force vectors
  forceCheck = new QCheckBox("draw force vectors",simLSetBox);
  forceCheck->setCheckState(Qt::Checked);
  connect(forceCheck, SIGNAL(stateChanged(int)), this,
      SLOT(forceCheckSlot(int)));

  writesimButton = new QToolButton(simLSetBox);
  writesimButton->setText( tr("Save simulation data") );
  connect(writesimButton, SIGNAL(clicked()), this, SLOT(writeSimSlot()));
  // DELETE THE POINTERS DELEETE THE POINTERS!:w
  //
  savesimagesButton = new QToolButton(simLSetBox);
  savesimagesButton->setText( tr("Save simulation images") );
  savesimagesButton->setToolTip( tr("Saves each frame of the animation as an image file. You can use a program like ffmpeg to create a video from the images.") );
  connect(savesimagesButton, SIGNAL(clicked()), this, SLOT(saveSimagesSlot()));

  //setup window
  simRenderWindow = vtkRenderWindow::New();
  //setup renderer
  simRenderer = vtkRenderer::New();
  simRenderWindow->AddRenderer(simRenderer);
  simQVTKW->SetRenderWindow(simRenderWindow);
  // USE CMAKE TO IDENTIFY TYPE OF COMPUTER? FOR VIDEO AVI OUTPUT
  qbikes->at(0)->initSim(simRenderer);

  simCallback = vtkSmartPointer<vtkTimerCallback2>::New();
  simCallback->qbikes = qbikes;
  simQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent, simCallback);
//  VTK_CREATE(vtkWindowToImageFilter,w2i);
//  VTK_CREATE(vtkJPEGWriter,writer);
//  VTK_CREATE(vtkPostScriptWriter,writer);
//  simRenderWindow->SetAAFrames(2);
  // Sign up to receive TimerEvent
 // VTK_CREATE(vtkTimerCallback2,callback);

  // plot

  simPlotQVTKW = new QVTKWidget(this);
  simPlotQVTKW->GetRenderWindow()->Render();
//    simCallback->simPlotVTKChart = simPlotVTKChart;
 //   simCallback->simPlotVTKView = simPlotVTKView;

//  simPlotCallback = vtkSmartPointer<vtkTimerCallback3>::New();
 // simPlotCallback->qbikes = qbikes;
 // simPlotQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent,
  //    simPlotCallback);
  
//  vtkPlot* simPlotVTKLine;
//  simPlotVTKLines.resize(NMOTIONVARS);

//  simPlotVTKChart->Paint(simPlotVTKView->GetContext());

//  simCallback->plotrenwin = simPlotQVTKW->GetRenderWindow();
 // simCallback->simPlotQVTKW = simPlotQVTKW;
  // arrange layouts
  // simLSetBox Widgets to simLayout
  simLSetLayout->addWidget(speedLabel,0,0,1,1);
  simLSetLayout->addWidget(speedEdit,0,1,1,1);
  simLSetLayout->addWidget(startsimButton,1,0);
  simLSetLayout->addWidget(stopsimButton,2,0);
  simLSetLayout->addWidget(forceCheck,3,0);
  simLSetLayout->addWidget(writesimButton,4,0);
  simLSetLayout->addWidget(savesimagesButton,5,0);
  
  simLayout->addWidget(simLSetBox,0,0);
  simLayout->addWidget(simQVTKW,0,1);
  simLayout->addWidget(simPlotQVTKW,1,0,1,2);
  setLayout(simLayout);
  
}


void myQSimTab::startsimSlot(void)
{
  // WHIPPLE CODE
  qbikes->at(0)->getBike()->evalConstants();
  qbikes->at(0)->getBike()->eoms();
  qbikes->at(0)->getBike()->computeOutputs();
  double state[10] = {qbikes->at(0)->getBike()->q0,
    qbikes->at(0)->getBike()->q1, qbikes->at(0)->getBike()->q3,
    qbikes->at(0)->getBike()->q4, qbikes->at(0)->getBike()->q5,
    qbikes->at(0)->getBike()->q6, qbikes->at(0)->getBike()->q7,
    qbikes->at(0)->getBike()->u1, qbikes->at(0)->getBike()->u3,
    qbikes->at(0)->getBike()->u5};
/*
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
  qbikes->at(0)->SimUpdate();
//  qbikes->at(0)->SetSimValues(0);
  simRenderer->SetBackground(.8,1,.8);
  simQVTKW->GetInteractor()->Initialize();
 
  simRenderer->AddActor(groundActor);

  // camera settings
  simRenderer->ResetCamera();
  simRenderer->GetActiveCamera()->SetPosition(0,1,-.5);
  simRenderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  simRenderer->GetActiveCamera()->Elevation(-95);
  // Render and interact
  //simQVTKW->GetRenderWindow()->Render();
 
  simCallback->SetState(state);
//  simCallback->writer = writer;
//  simCallback->w2i = w2i;
 
  int timerId = simQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/qbikes->at(0)->getBike()->fps);
  std::cout << "timerId: " << timerId << std::endl;
 
  // Start the interaction and timer
  simQVTKW->GetInteractor()->Start();
//  delete qbikes->at(0);
/*
  int timerId3 = simPlotQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/qbikes->at(0)->getBike()->fps);
  std::cout << "timerId3: " << timerId3 << std::endl;
 
  // Start the interaction and timer*/
  //simPlotQVTKW->GetInteractor()->Start();
}

void myQSimTab::stopsimSlot(void)
{
  simQVTKW->GetInteractor()->DestroyTimer();
  simPlotVTKView = vtkSmartPointer<vtkContextView>::New();
  simPlotVTKView->SetInteractor(simPlotQVTKW->GetInteractor());
  simPlotQVTKW->SetRenderWindow(simPlotVTKView->GetRenderWindow());
  simPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  simPlotVTKView->GetScene()->AddItem(simPlotVTKChart);
  simPlotVTKChart->ClearPlots();
  qbikes->at(0)->GetSimTable()->RemoveRow(qbikes->at(0)->GetSimTable()->GetNumberOfRows());
  for (int i = 1; i < NMOTIONVARS; i++) {
/*    simPlotVTKLine = simPlotVTKChart->AddPlot(vtkChart::LINE);
    simPlotVTKLine->SetInput(qbikes->at(0)->GetSimTable(), 0, i);
    simPlotVTKLine->SetColor(255,0,0,255);*/
    // ideally, create 32 vtkPlot simPlotVTKLines
    simPlotVTKChart->AddPlot(vtkChart::LINE);
    simPlotVTKChart->GetPlot(i-1)->SetInput(qbikes->at(0)->GetSimTable(),0,i);
    simPlotVTKChart->GetPlot(i-1)->SetColor(255,0,0,255);
  }

  qbikes->at(0)->GetSimTable()->Update();
  simPlotVTKChart->Update();
  simPlotQVTKW->GetInteractor()->Initialize();
  simPlotQVTKW->GetRenderWindow()->Render();
  simPlotQVTKW->GetInteractor()->Start();

  qbikes->at(0)->UpdateTrace();
  simRenderWindow->Render();
  //simPlotVTKChart->Paint(simPlotVTKView->GetContext());
}

void myQSimTab::updatePlotSlot(void)
{
}

// CAN'T HIT START ANIMATION TWICE!!
void myQSimTab::forceCheckSlot(int state)
{
  if (state == Qt::Checked) {
    qbikes->at(0)->TurnOnReactionTriads();
  } else if (state == Qt::Unchecked) {
    qbikes->at(0)->TurnOffReactionTriads();
  }
}

void myQSimTab::writeSimSlot(void)
{
  QString fname = QFileDialog::getSaveFileName(this, tr("Save File"),
      QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)")
      );
  qbikes->at(0)->writeSim(fname.toStdString());
}

void myQSimTab::saveSimagesSlot(void)
{
  QString fname = QFileDialog::getSaveFileName(this, tr("Save File"),
      QDir::currentPath(), tr("PNG file (*.png)"));


}

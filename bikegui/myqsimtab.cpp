// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>
#include <QVTKWidget.h>

// vtk
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAssembly.h>
#include <vtkCamera.h>
#include <vtkChartXY.h>
#include <vtkCommand.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkJPEGWriter.h>
#include <vtkPlaneSource.h>
#include <vtkPlot.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPostScriptWriter.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkTable.h>
#include <vtkTransform.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkWindowToImageFilter.h>

// OBD
#include "whipple.h"

// gui
#include "myqsimtab.h"
#include "myvtkTriad.h"
#include "myqwhipple.h"

// define
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
#define NMOTIONVARS 32

MyQSimTab::MyQSimTab(std::vector<MyQWhipple*>* qb, QWidget *parent) :
  QWidget(parent) {

  qbikes = qb;
  simLSetBox = new QGroupBox("Parameters", this);
  simLSetLayout = new QGridLayout(simLSetBox);
  //  QGroupBox *simRSetBox = new
	simLayout = new QGridLayout(this);

  simQVTKW = new QVTKWidget(this);
  //  simQVTKW->resize(256, 256);

  // initial velocity
  speedLabel = new QLabel( tr("initial speed (m/s)"), simLSetBox);
  speedEdit = new QLineEdit(simLSetBox);

  // simLSetBox
  startsimButton = new QToolButton(simLSetBox);
  startsimButton->setText( tr("Start simulation") );
  connect(startsimButton, SIGNAL(clicked()), this, SLOT(startsimSlot()) );

  stopsimButton = new QToolButton(simLSetBox);
  stopsimButton->setText( tr("Stop simulation") );
  connect(stopsimButton, SIGNAL(clicked()), this, SLOT(stopsimSlot()));

  // include force vectors
  forceCheck = new QCheckBox("draw force vectors", simLSetBox);
  forceCheck->setCheckState(Qt::Checked);
  connect(forceCheck, SIGNAL(stateChanged(int)), this,
      SLOT(forceCheckSlot(int)));

  writesimButton = new QToolButton(simLSetBox);
  writesimButton->setText( tr("Save simulation data") );
  connect(writesimButton, SIGNAL(clicked()), this, SLOT(printSimSlot()));
  // DELETE THE POINTERS DELEETE THE POINTERS!

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

  simCallback = vtkSmartPointer<vtkTimerCallback2>::New();
  simCallback->qbikes = qbikes;
  simQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent, simCallback);
//  VTK_CREATE(vtkWindowToImageFilter, w2i);
//  VTK_CREATE(vtkJPEGWriter, writer);
//  VTK_CREATE(vtkPostScriptWriter, writer);
//  simRenderWindow->SetAAFrames(2);

  // plot

  simPlotQVTKW = new QVTKWidget(this);
  simPlotQVTKW->GetRenderWindow()->Render();
//    simCallback->simPlotVTKChart = simPlotVTKChart;
 //   simCallback->simPlotVTKView = simPlotVTKView;

//  simPlotCallback = vtkSmartPointer<vtkTimerCallback3>::New();
 // simPlotCallback->qbikes = qbikes;
 // simPlotQVTKW->GetInteractor()->AddObserver(vtkCommand::TimerEvent,
  //    simPlotCallback);

//  vtkPlot *simPlotVTKLine;
//  simPlotVTKLines.resize(NMOTIONVARS);

//  simPlotVTKChart->Paint(simPlotVTKView->GetContext());

//  simCallback->plotrenwin = simPlotQVTKW->GetRenderWindow();
 // simCallback->simPlotQVTKW = simPlotQVTKW;
  // arrange layouts
  // simLSetBox Widgets to simLayout
  simLSetLayout->addWidget(speedLabel, 0, 0, 1, 1);
  simLSetLayout->addWidget(speedEdit, 0, 1, 1, 1);
  simLSetLayout->addWidget(startsimButton, 1, 0);
  simLSetLayout->addWidget(stopsimButton, 2, 0);
  simLSetLayout->addWidget(forceCheck, 3, 0);
  simLSetLayout->addWidget(writesimButton, 4, 0);
  simLSetLayout->addWidget(savesimagesButton, 5, 0);

  simLayout->addWidget(simLSetBox, 0, 0);
  simLayout->addWidget(simQVTKW, 0, 1);
  simLayout->addWidget(simPlotQVTKW, 1, 0, 1, 2);
  setLayout(simLayout);

} // startsimSlot()


void MyQSimTab::startsimSlot(void) {

  int ndobikes = 0;
  for (unsigned int i = 0; i < qbikes->size(); i++) {
    if (qbikes->at(i)->getDoSim()) {
      bidx = i;
      ndobikes++;
    }
  }
  if (ndobikes == 0) {
    QMessageBox::information(this, tr("No bicycle selected for simulation."), tr("No bicycle selected for simulation. Select one bicycle from the Bicycle Library tab."));
  }
  if (ndobikes > 1) {
    QMessageBox::information(this, tr("More than one bicycle selected for simulation."), tr("More than one bicycle is selected for simulation. Select only one bicycle from the Bicycle Library tab."));
  }
  qbikes->at(bidx)->initSim(simRenderer);

  // WHIPPLE CODE
/*
  // MAKE HUD-DISPLAY, TEXTACTOR:w
  VTK_CREATE(vtkTextProperty, text1Propr);
  text1Propr->SetFontSize(10);
  text1Propr->SetJustificationToCentered();
  VTK_CREATE(vtkTextMapper, text1Mapper);
  VTK_CREATE(vtkActor2D, text1Actor2D);
    // CAN I LUMP TOGETHER DIFFERENT "SOURCES" INTO ONE ACTOR?"
  simRenderer->SetBackground(1, 1, 1);
  simRenderer->ResetCamera();
*/
  // draw coordinate triad
  myvtkTriad triad0(simRenderer);
  // ground
  VTK_CREATE(vtkPlaneSource, groundSource);
  groundSource->SetNormal(0, 0, -1);
  groundSource->Update();
  VTK_CREATE(vtkPolyDataMapper, groundMapper);
  groundMapper->SetInputConnection(groundSource->GetOutputPort());
  VTK_CREATE(vtkActor, groundActor);
  groundActor->SetMapper(groundMapper);
  groundActor->SetScale(3, 3, 3);
  groundActor->GetProperty()->SetColor(0, 0, 0);
  groundActor->GetProperty()->SetOpacity(0.5);
  // draw a bike
  qbikes->at(bidx)->UpdateSim(0);
//  qbikes->at(bidx)->SetSimValues(0);
  simRenderer->SetBackground(.8, 1,.8);
  simQVTKW->GetInteractor()->Initialize();

  simRenderer->AddActor(groundActor);

  // camera settings
  simRenderer->ResetCamera();
  simRenderer->GetActiveCamera()->SetPosition(0, 1, -.5);
  simRenderer->GetActiveCamera()->SetViewUp(0, 0, -1);
//  simRenderer->GetActiveCamera()->Elevation(-95);
  // Render and interact
  //simQVTKW->GetRenderWindow()->Render();

//  simCallback->writer = writer;
//  simCallback->w2i = w2i;

  int timerId = simQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/qbikes->at(bidx)->getBike()->fps);
  std::cout << "timerId: " << timerId << std::endl;

  // Start the interaction and timer
  simQVTKW->GetInteractor()->Start();
/*
  int timerId3 = simPlotQVTKW->GetInteractor()->
      CreateRepeatingTimer(1000/qbikes->at(bidx)->getBike()->fps);
  std::cout << "timerId3: " << timerId3 << std::endl;

  // Start the interaction and timer*/
  //simPlotQVTKW->GetInteractor()->Start();
}

void MyQSimTab::stopsimSlot(void) {
  simQVTKW->GetInteractor()->DestroyTimer();

  // delete everything initialized by MyQWhipple::initSim()

  simPlotVTKView = vtkSmartPointer<vtkContextView>::New();
  simPlotVTKView->SetInteractor(simPlotQVTKW->GetInteractor());
  simPlotQVTKW->SetRenderWindow(simPlotVTKView->GetRenderWindow());
  simPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  simPlotVTKView->GetScene()->AddItem(simPlotVTKChart);
  simPlotVTKChart->ClearPlots();
  qbikes->at(bidx)->GetSimTable()->RemoveRow(qbikes->at(bidx)->GetSimTable()->GetNumberOfRows());
  for (int i = 1; i < NMOTIONVARS; i++) {
/*    simPlotVTKLine = simPlotVTKChart->AddPlot(vtkChart::LINE);
    simPlotVTKLine->SetInput(qbikes->at(bidx)->GetSimTable(), 0, i);
    simPlotVTKLine->SetColor(255, 0, 0, 255);*/
    // ideally, create 32 vtkPlot simPlotVTKLines
    simPlotVTKChart->AddPlot(vtkChart::LINE);
    simPlotVTKChart->GetPlot(i-1)->SetInput(qbikes->at(bidx)->GetSimTable(), 0, i);
    simPlotVTKChart->GetPlot(i-1)->SetColor(255, 0, 0, 255);
  } // for i

  qbikes->at(bidx)->GetSimTable()->Update();
  simPlotVTKChart->Update();
  simPlotQVTKW->GetInteractor()->Initialize();
  simPlotQVTKW->GetRenderWindow()->Render();
  simPlotQVTKW->GetInteractor()->Start();

  qbikes->at(bidx)->UpdateTrace();
  simRenderWindow->Render();
  //simPlotVTKChart->Paint(simPlotVTKView->GetContext());
} // stopSimSlot()

void MyQSimTab::updatePlotSlot(void) {
} // updatePlotSlot()

// CAN'T HIT START ANIMATION TWICE!!
void MyQSimTab::forceCheckSlot(int state) {
  if (state == Qt::Checked) {
    qbikes->at(bidx)->TurnOnReactionTriads();
  } else if (state == Qt::Unchecked) {
    qbikes->at(bidx)->TurnOffReactionTriads();
  } // if
} // forceCheckSlot()

void MyQSimTab::printSimSlot(void) {
  QString fname = QFileDialog::getSaveFileName(this, tr("Save File"),
      QDir::currentPath(), tr("Text file (*, *.txt, *.dat,...);;Any file (*)")
      );
  qbikes->at(bidx)->printSimData(fname.toStdString());
} // printSimSlot()

void MyQSimTab::saveSimagesSlot(void) {
  QString fname = QFileDialog::getSaveFileName(this, tr("Save File"),
      QDir::currentPath(), tr("PNG file (*.png)"));
// page 247 of vtkusermanual: OffScreenRenderingOn(), and RenderLargeImage,
// specify magnification/ there is a
// Examples/Rendering/Tcl/RenderLargeImage.tcl example
} // saveSimagesSlot()

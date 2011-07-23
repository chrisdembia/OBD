#include <cstdlib>
#include <string>
#include <vector>

#include <QtGui>
// graphics

#include <QVTKWidget.h>
// plotting
#include "vtkQtLineChartView.h"
#include "vtkQtChartRepresentation.h"
#include "vtkQtTableView.h"
#include "vtkDataObjectToTable.h"
#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include <vtkMath.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>

#include <getopt.h>
#include "gslVecUtils.h"
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"

#include "parameters.h"
#include "mainwindow.h"

std::ostream &operator<<(std::ostream &outfile, const Whipple * discs);
// constructor
MainWindow::MainWindow()
{
  // Set MainWindow parameterss
  setAnimated(true);
  setDockNestingEnabled(true);
  setWindowTitle(tr("Open Bicycle Dynamics"));

  // Allocate space for a Whipple object
  bike = new Whipple();

  // Set version string
  versionString = QString(tr("Version %1.%2 commit %3"))
              .arg(OBD_VERSION_MAJOR)
              .arg(OBD_VERSION_MINOR)
              .arg(OBD_VERSION_COMMIT);

  // Set up Actions, Menus, Tabs, Docks
  createActions();
  createMenus();
  createDockWindows();
  createStatusBar();
  createTabs();

  //  QVBoxLayout 
  setCentralWidget(tabWidget);
}

MainWindow::~MainWindow() {

}

void MainWindow::about(void)
{
  QString test = QString(tr("<h2>Open Bicycle Dynamics</h2>"
        "%1"
        "<p>Copyright &copy; 2010-2011 Dale Lukas Peterson</p>"
        "<p><a href='mailto:%2'>%2</a>"))
        .arg(versionString, OBD_AUTHOR_CONTACT);
  QMessageBox::about(this, tr("About OBD"), test);
}

void MainWindow::createActions(void)
{
  // New action
  newAction = new QAction(tr("&New"), this);
  newAction->setShortcut(QKeySequence::New);
  newAction->setStatusTip(tr("Create a new set of bicycle parameters"));
  // connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

  // Open action
  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcut(QKeySequence::Open);
  openAction->setStatusTip(tr("Open a set of bicycle parameters"));
  // connect(openAction, SIGNAL(triggered()), this, SLOT(about()));

  // Save action
  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcut(QKeySequence::Save);
  saveAction->setStatusTip(tr("Save bicycle parameters to file"));
  // connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  // Save as action
  saveAsAction = new QAction(tr("Save &As"), this);
  saveAsAction->setShortcut(QKeySequence::SaveAs);
  saveAsAction->setStatusTip(tr("Save bicycle parameters to file "));
  // connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

  // Quit action
  quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcut(QKeySequence::Quit);
  quitAction->setStatusTip(tr("Quit"));
  connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

  // About action
  aboutAction = new QAction(tr("&About"), this);
  aboutAction->setStatusTip(tr("About OBD"));
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

  // About Qt action
  aboutQtAction = new QAction(tr("About &Qt"), this);
  aboutQtAction->setStatusTip(tr("About Qt"));
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createDockWindows(void)
{
  QDockWidget *dock = new QDockWidget(tr("Bike Definitions"));
  dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
  paramWidget = new WhippleParameter(bike, dock);
  dock->setWidget(paramWidget);
  
}

void MainWindow::createMenus(void)
{
  // Add the file menu
  fileMenu = menuBar()->addMenu(tr("&File"));
  // Add actions to file menu
  fileMenu->addAction(newAction);
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(saveAsAction);
  fileMenu->addAction(quitAction);

  // Add the help menu
  helpMenu = menuBar()->addMenu(tr("&Help"));
  // Add actions to help menu
  helpMenu->addAction(aboutAction);
  helpMenu->addAction(aboutQtAction);
}

void MainWindow::createStatusBar(void)
{
  statusBar()->showMessage(versionString);
}

void MainWindow::createTabs(void)
{

//  msgLabel = new QLabel;

  uprightStabilityTab = new QWidget;
  steadyTurningTab = new QWidget;
  motionVisualizationTab = new QWidget;
  
  tabWidget = new QTabWidget();
  tabWidget->addTab( uprightStabilityTab, tr("Upright stability"));
  tabWidget->addTab( steadyTurningTab, tr("Steady turning"));
  tabWidget->addTab( motionVisualizationTab, tr("Motion visualization"));
  /*
  uprightStabilityTab->setParent(tabWidget);
  steadyTurningWidget->setParent(tabWidget);
  motionVisualizationWidget->setParent(tabWidget);
  */
  
  createUprightStabilityTab();
  createSteadyTurningTab();
  createMotionVisualizationTab();
}

void MainWindow::createUprightStabilityTab(void)
{

  // upOpts is a structure for run parameters for upright stability eigenvalue plots
  upOpts.outfolder[0] = '\0';
  upOpts.N = 201;
  upOpts.pitchguess = 0.0;
  upOpts.vi = 0.0;
  upOpts.vf = 10.0;  
  
  QGridLayout *uprightSetLayout = new QGridLayout;
//  uprightSetLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  uprightSetLayout->setVerticalSpacing(0);  
  QToolButton * updateEigButton = new QToolButton(uprightStabilityTab);
  updateEigButton->setText( tr("Update eigenvalue plot") );
  connect(updateEigButton, SIGNAL(clicked()), this, SLOT(updateEigPlotSlot()) );
  uprightSetLayout->addWidget(updateEigButton,0,0);
  
  // manage options
  
  uprightSetLayout->addWidget(new QLabel( tr("Save eigenvalue data"),0,0 ) );
  
  // Save button
  QToolButton * saveEigButton = new QToolButton(uprightStabilityTab);
  saveEigButton->setText( tr("Save Plot") );
  connect(saveEigButton, SIGNAL(clicked()), this, SLOT(saveEigSlot() ) );
  uprightSetLayout->addWidget(saveEigButton,0,1,Qt::AlignRight);
  
  // Edit filename for saving plot
  saveEigFilenameEdit = new QLineEdit( tr( upOpts.outfolder.c_str() ) );
  saveEigFilenameEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(saveEigFilenameEdit,1,0,1,2);
  
  // Number of evaluation points
  uprightSetLayout->addWidget(new QLabel( tr("number of evaluation points") ),3,0);
  nEvalPointsEdit = new QLineEdit;
  nEvalPointsEdit->setText( QString("%1").arg(upOpts.N) );
  nEvalPointsEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(nEvalPointsEdit,3,1);
  
  // Pitch guess
  uprightSetLayout->addWidget(new QLabel( tr("pitch guess (rad) (blank for default)") ),2,0);
  pitchGuessEdit = new QLineEdit;
  pitchGuessEdit->setText( QString("%1").arg(upOpts.pitchguess) );
  pitchGuessEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(pitchGuessEdit,2,1);
  
  // Initial speed
  uprightSetLayout->addWidget(new QLabel( tr("initial speed v_i (m/s)") ), 4,0);
  initSpeedEdit = new QLineEdit;
  initSpeedEdit->setText( QString("%1").arg(upOpts.vi) );
  initSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(initSpeedEdit,4,1);
  
  // Final speed
  uprightSetLayout->addWidget(new QLabel( tr("final speed v_f (m/s)") ), 5,0);
  finalSpeedEdit = new QLineEdit;
  finalSpeedEdit->setText( QString("%1").arg(upOpts.vf) );
  finalSpeedEdit->setAlignment(Qt::AlignRight);
  uprightSetLayout->addWidget(finalSpeedEdit,5,1);

QGroupBox *uprightSetBox = new QGroupBox( tr("Settings") );

uprightSetBox->setLayout(uprightSetLayout);
uprightSetBox->setMaximumWidth(200);
 //QScrollArea *eigScroll = new QScrollArea;
 //eigScroll->setBackgroundRole(QPalette::Dark);
  
QHBoxLayout *uprightTopLayout = new QHBoxLayout;
    uprightTopLayout->addWidget(uprightSetBox);
  // QVTK set up and initialization
  eigPlotQVTKW = new QVTKWidget(uprightStabilityTab);
//  chartView->GetWidget()->setMinimumSize(800,800);
//        uprightTopLayout->addWidget(chartView->GetWidget());
//QGroupBox *eigVTKBox = new QGroupBox( tr("heyyo") );
//eigVTKBox->setLayout(eigPlotLayout);
    uprightTopLayout->addWidget(eigPlotQVTKW);  
  uprightStabilityTab->setLayout(uprightTopLayout);

eigPlotQVTKW->setMinimumSize(500,200);
//    return 0; for errors?
}

void MainWindow::createSteadyTurningTab(void)
{
  QGridLayout *steadyLayout = new QGridLayout;

  QComboBox *bikeDropDown = new QComboBox;
  bikeDropDown->addItem( tr("bike1") );
  
    
  steadyLayout->addWidget(bikeDropDown);
  steadyTurningTab->setLayout(steadyLayout);
}

void MainWindow::createMotionVisualizationTab(void)
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

void MainWindow::saveEigSlot(void)
{
  /*QFileDialog *saveEigDirDial = new QFileDialog(uprightStabilityTab);
  saveEigDirDial->setFileMode(QFileDialog::Directory);
  saveEigDirDial->setViewMode(QFileDialog::List);
  */
  QString dir = QFileDialog::getExistingDirectory(this,tr("Choose Directory"),QDir::currentPath(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

}


void MainWindow::updateEigPlotSlot(void)
{
  // THIS STUFF BELOW IS MOSTLY COPIED
  // Set up my 2D world...
  VTK_CREATE(vtkContextView, eigPlotVTKView); // This contains a chart object
  eigPlotVTKView->SetInteractor(eigPlotQVTKW->GetInteractor());
  eigPlotQVTKW->SetRenderWindow(eigPlotVTKView->GetRenderWindow());
  
  // Create a table with some points in it...
  VTK_CREATE(vtkTable, eigPlotVTKTable);
  VTK_CREATE(vtkFloatArray, arrX);
  arrX->SetName("forward velocity (m/s)");
  eigPlotVTKTable->AddColumn(arrX);
  
//  int Nbikes = 1;
  int NeigPerBike = 4;
  std::vector<std::string> bikenames(paramWidget->getNbikes()*NeigPerBike);
  bikenames[0] = "eig1";
  bikenames[1] = "eig2";
  bikenames[2] = "eig3";
  bikenames[3] = "eig4";

  std::vector< vtkSmartPointer<vtkFloatArray> > arrY(paramWidget->getNbikes()*NeigPerBike); 
  int idx;
  for (int i = 0; i < paramWidget->getNbikes(); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      idx = j + i*paramWidget->getNbikes();
      arrY[idx] = vtkSmartPointer<vtkFloatArray>::New();
      arrY[idx]->SetName(bikenames[j].c_str());
      eigPlotVTKTable->AddColumn(arrY[idx]);
    }
  }   

// WHIPPLE WHIPPLE WHIPPLE WHIPPLE 
  std::string filename;
  upOpts.outfolder = saveEigFilenameEdit->text().toStdString();
  upOpts.N = nEvalPointsEdit->text().toInt();
  upOpts.pitchguess = pitchGuessEdit->text().toDouble();
  upOpts.vi = initSpeedEdit->text().toDouble();
  upOpts.vf = finalSpeedEdit->text().toDouble();  

// TIME TO GRAB PARAMETERS FROM PARAMETER WIDGET!! must validate them, yo.

  bike->evalConstants();
  bike->eoms();
  bike->computeOutputs();

  // Write parameters
  
  filename = upOpts.outfolder; filename += "eigenvalue_parameters.txt";
  bike->writeParameters(filename.c_str());
  // Write data record file. the function is orphaned from whipple.h currently
  // allows the evaluation of data by python
//  filename = upOpts.outfolder; filename += "eval_record.py";
//  writeEvalRecord_dt(filename.c_str());
  // Open data file
  filename = upOpts.outfolder; filename += "eigenvalues.dat";
  std::ofstream OutputFile( filename.c_str() ); // std::ios::binary);
  
  // Vector to store range of speeds to calculate eigenvalues
  gsl_vector * speed = linspaceN( upOpts.vi, upOpts.vf, upOpts.N);

  bike->u1 = 0.0;
  bike->u3 = 0.0;
  bike->evalConstants();

  eigPlotVTKTable->SetNumberOfRows(upOpts.N);
  for (int i = 0; i < upOpts.N; ++i) {
    bike->u5 = -gsl_vector_get(speed, i)/(bike->rf+bike->rft);
    bike->calcEvals();
    OutputFile << *gsl_vector_ptr(speed,i);
    eigPlotVTKTable->SetValue(i, 0, *gsl_vector_ptr(speed,i) );
    for (int j = 0; j < 4; ++j)
    {
      OutputFile << " " << bike->fourValues[j];
      eigPlotVTKTable->SetValue(i, j+1, bike->fourValues[j]);
    }
    OutputFile << std::endl;
//    OutputFile.write((char *) gsl_vector_ptr(speed, i), sizeof(double));
//    OutputFile.write((char *) bike->fourValues, 4*sizeof(double));  
  } // for i




  // QVTK QVTK QVTK QVTK 
  eigPlotVTKTable->Update();
  
  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> eigPlotVTKChart = vtkSmartPointer<vtkChartXY>::New();
  eigPlotVTKView->GetScene()->AddItem(eigPlotVTKChart);
  vtkPlot *eigPlotVTKLine;

  for (int i = 0; i < paramWidget->getNbikes(); i++)
  {
    for (int j = 0; j < 4; j++)
    {
      idx = j + i*paramWidget->getNbikes();
      eigPlotVTKLine = eigPlotVTKChart->AddPlot(vtkChart::LINE);
      eigPlotVTKLine->SetInput(eigPlotVTKTable, 0, j+1);
      eigPlotVTKLine->SetColor(0, 255, 0, 255);
      eigPlotVTKLine->SetWidth(2.0);
    }
  }





  // WHIPPLE WHIPPLE WHIPPLE
  std::cout << "Eigenvalue data written to ";
  if ( upOpts.outfolder.empty() )
  { 
  	std::cout << "current directory." << std::endl;
  }
  else
  {
  	std::cout << upOpts.outfolder << std::endl;
  }

  // Close files and free memory
  OutputFile.close();
  gsl_vector_free(speed);

  // SHOULD MAKE DESTRUCTORS

}

std::ostream &operator<<(std::ostream &file, const Whipple * discs)
{
  file.write((char *) &(discs->t), sizeof discs->t);
  file.write((char *) &discs->q0, sizeof discs->q0);
  file.write((char *) &discs->q1, sizeof discs->q1);
  file.write((char *) &discs->q2, sizeof discs->q2);
  file.write((char *) &discs->q3, sizeof discs->q3);
  file.write((char *) &discs->q4, sizeof discs->q4);
  file.write((char *) &discs->q5, sizeof discs->q5);
  file.write((char *) &discs->q6, sizeof discs->q6);
  file.write((char *) &discs->q7, sizeof discs->q7);
  file.write((char *) &discs->u0, sizeof discs->u0);
  file.write((char *) &discs->u1, sizeof discs->u1);
  file.write((char *) &discs->u2, sizeof discs->u2);
  file.write((char *) &discs->u3, sizeof discs->u3);
  file.write((char *) &discs->u4, sizeof discs->u4);
  file.write((char *) &discs->u5, sizeof discs->u5);
  file.write((char *) discs->no_fn, sizeof discs->no_fn);
  file.write((char *) &discs->Rx, sizeof discs->Rx);
  file.write((char *) &discs->Ry, sizeof discs->Ry);
  file.write((char *) &discs->Rz, sizeof discs->Rz);
  file.write((char *) &discs->Fx, sizeof discs->Fx);
  file.write((char *) &discs->Fy, sizeof discs->Fy);
  file.write((char *) &discs->Fz, sizeof discs->Fz);
  file.write((char *) &discs->ke, sizeof discs->ke);
  file.write((char *) &discs->pe, sizeof discs->pe);
  file.write((char *) &discs->fa_yaw, sizeof discs->fa_yaw);
  file.write((char *) &discs->fa_lean, sizeof discs->fa_lean);
  file.write((char *) &discs->fa_pitch, sizeof discs->fa_pitch);
  file.write((char *) discs->constraints, sizeof discs->constraints);
  return file;
} // ostream &operator<<()
// VTK
/*
  // Create a table with some points in it
  vtkSmartPointer<vtkTable> table = 
    vtkSmartPointer<vtkTable>::New();
 
  vtkSmartPointer<vtkFloatArray> arrX = 
    vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("X Axis");
  table->AddColumn(arrX);
 
  vtkSmartPointer<vtkFloatArray> arrC = 
    vtkSmartPointer<vtkFloatArray>::New();
  arrC->SetName("Cosine");
  table->AddColumn(arrC);
 
  vtkSmartPointer<vtkFloatArray> arrS = 
    vtkSmartPointer<vtkFloatArray>::New();
  arrS->SetName("Sine");
  table->AddColumn(arrS);
 
  // Fill in the table with some example values
  int numPoints = 69;
  float inc = 7.5 / (numPoints-1);
  table->SetNumberOfRows(numPoints);
  for (int i = 0; i < numPoints; ++i)
  {
    table->SetValue(i, 0, i * inc);
    table->SetValue(i, 1, cos(i * inc));
    table->SetValue(i, 2, sin(i * inc));
  }
 
  // Set up the view
  vtkSmartPointer<vtkContextView> view = 
    vtkSmartPointer<vtkContextView>::New();
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
 
  // Add multiple line plots, setting the colors etc
  vtkSmartPointer<vtkChartXY> chart = 
    vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  vtkPlot *line = chart->AddPlot(vtkChart::LINE);
  line->SetInput(table, 0, 1);
  line->SetColor(0, 255, 0, 255);
  line->SetWidth(1.0);
  line = chart->AddPlot(vtkChart::LINE);
  line->SetInput(table, 0, 2);
  line->SetColor(255, 0, 0, 255);
  line->SetWidth(5.0);
  
    // Start interactor
  view->GetInteractor()->Initialize();
 view->GetInteractor()->Start();
  
*/
 /*
 // Create a sphere and create a vtkTable from its point data (normal vectors)
  VTK_CREATE(vtkSphereSource, sphereSource);
  VTK_CREATE(vtkDataObjectToTable, tableConverter);
  tableConverter->SetInput(sphereSource->GetOutput());
  tableConverter->SetFieldType(vtkDataObjectToTable::POINT_DATA);
  tableConverter->Update();
  vtkTable* pointTable = tableConverter->GetOutput();

  // Create a line chart view
  vtkSmartPointer<vtkQtLineChartView> chartView = 
    vtkSmartPointer<vtkQtLineChartView>::New();
  chartView->SetupDefaultInteractor();
  
//  eigScroll->setWidget(chartView->GetWidget());
//    uprightTopLayout->addWidget(eigScroll);

  // Set the chart title
  chartView->SetTitle("Sphere Normals");

  // Add the table to the view
  vtkDataRepresentation* dataRep = chartView->AddRepresentationFromInput(pointTable);

  // You can downcast to get the chart representation:
  vtkQtChartRepresentation* chartRep =
    vtkQtChartRepresentation::SafeDownCast(dataRep);
  if (!chartRep)
    {
    std::cerr << "Failed to get chart table representation." << std::endl;
//    return 1;
    }

  // TODO-
  // The user shouldn't be required to call Update().
  // The view should handle updates automatically. 
  chartView->Update();

  // Show the view's qt widget
  chartView->Show();
  // eigScroll->setWidget(eigPlot); 
*/

//     QLabel * eigPlot = new QLabel("HELLO");
//  QImage image("human.png");
//  eigPlot->setPixmap(QPixmap::fromImage(image));
//         eigPlot->setMinimumSize(200,200);
    /*QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap("yellow.gif"));
    scene.addItem(&item);
    view.show();*/
//eigPlot->setBackgroundRole(QPalette::Base);
//eigPlot->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//eigPlot->setScaledContents(true);
  //QGraphicsView
  //QCanvas


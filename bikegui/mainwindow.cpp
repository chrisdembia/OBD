#include <cstdlib>
#include <QtGui>
#include "parameters.h"
#include "button.h"
#include "mainwindow.h"
#include "OBDConfig.h"
#include "whipple.h"
#include <getopt.h>
#include "gslVecUtils.h"
#include "whippleutils.h"
//#include "QVTKWidget.h"
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
  QDockWidget *dock = new QDockWidget(tr("Parameters"));
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
  
//  tabWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
/*  QScrollArea *  scrollArea = new QScrollArea;
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(tabWidget);
  scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  tabWidget->setMinimumSize(200,200);*/

  createUprightStabilityTab();
  createMotionVisualizationTab();
  
  setCentralWidget(tabWidget);
}
/*
Button *MainWindow::createButton(const QString &text, const char *member)
{
   //  Button *button = new Button(text);
   // connect(button, SIGNAL(clicked()), this, member);
     return void; //button;
}
*/

void MainWindow::createMotionVisualizationTab(void)
{
	QLabel *l1 = new QLabel( tr("TESTING") );
	QGridLayout *motionLayout = new QGridLayout;
	motionLayout->addWidget(l1);
	motionVisualizationTab->setLayout(motionLayout);
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
  uprightSetLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  
  QToolButton * updateEigButton = new QToolButton(uprightStabilityTab);
  updateEigButton->setText( tr("Update eigenvalue plot") );
//  Button * updateEigButton = createButton(tr("Update eigenvalue plot"), SLOT(updateEigPlot()));
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
  
  QHBoxLayout *uprightTopLayout = new QHBoxLayout;
     QLabel * eigPlot = new QLabel("HELLO");
     
  QImage image("yellow.gif");
  eigPlot->setPixmap(QPixmap::fromImage(image));
         eigPlot->setMinimumSize(200,200);
         /*
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap("yellow.gif"));
    scene.addItem(&item);
    view.show();*/
     //eigPlot->setBackgroundRole(QPalette::Base);
     //eigPlot->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
     //eigPlot->setScaledContents(true);
  //QGraphicsView
  //QCanvas
     
QGroupBox *uprightSetBox = new QGroupBox( tr("Settings") );
uprightSetBox->setLayout(uprightSetLayout);
  
uprightTopLayout->addWidget(uprightSetBox);
 uprightTopLayout->addWidget(eigPlot);


  
  uprightStabilityTab->setLayout(uprightTopLayout);
//    return 0; for errors?
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
  std::cout << "STOP PRESSING ME!" << std::endl;

  // THIS STUFF BELOW IS MOSTLY COPIED
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
  /*
  filename = upOpts.outfolder; filename += "eigenvalue_parameters.txt";
  bike->writeParameters(filename.c_str());*/
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

  for (int i = 0; i < upOpts.N; ++i) {
    bike->u5 = -gsl_vector_get(speed, i)/(bike->rf+bike->rft);
    bike->calcEvals();
    OutputFile << *gsl_vector_ptr(speed,i);
	for (int j = 0; j < 4; ++j)
    {
	  OutputFile << " " << bike->fourValues[j];
    }
    OutputFile << std::endl;
//    OutputFile.write((char *) gsl_vector_ptr(speed, i), sizeof(double));
//    OutputFile.write((char *) bike->fourValues, 4*sizeof(double));  
    } // for i

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

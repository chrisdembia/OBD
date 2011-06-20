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

using namespace std;

MainWindow::MainWindow()
{
  // Set MainWindow parameters
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
  createUprightStabilityTab();
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
  paramWidget = new WhippleParameter(dock);
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

  uprightStabilityWidget = new QWidget;
  steadyTurningWidget = new QWidget;
  motionVisualizationWidget = new QWidget;
  
  tabWidget = new QTabWidget();
  tabWidget->addTab( uprightStabilityWidget, tr("Upright stability"));
  tabWidget->addTab( steadyTurningWidget, tr("Steady turning"));
  tabWidget->addTab( motionVisualizationWidget, tr("Motion visualization"));
  /*
  uprightStabilityWidget->setParent(tabWidget);
  steadyTurningWidget->setParent(tabWidget);
  motionVisualizationWidget->setParent(tabWidget);
  */
  
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

void MainWindow::createUprightStabilityTab(void)
{
  QGridLayout *uprightStabilityLayout = new QGridLayout;
  uprightStabilityLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
  
  QToolButton * updateEigButton = new QToolButton(uprightStabilityWidget);
  updateEigButton->setText( tr("Update eigenvalue plot") );
//  Button * updateEigButton = createButton(tr("Update eigenvalue plot"), SLOT(updateEigPlot()));
  connect(updateEigButton, SIGNAL(clicked()), this, SLOT(updateEigPlot()) );

  uprightStabilityLayout->addWidget(updateEigButton,0,0);
  
  // manage options
  
  uprightStabilityLayout->addWidget(new QLabel( tr("Save eigenvalue data"),0,0 ) );
  
  // i want to right-justify this button
  QToolButton * saveEigButton = new QToolButton(uprightStabilityWidget);
  saveEigButton->setText( tr("Save") );
  uprightStabilityLayout->addWidget(saveEigButton,0,1,Qt::AlignRight);
  
  QLineEdit *saveEigFilenameEdit = new QLineEdit( tr("filename") );
  saveEigFilenameEdit->setAlignment(Qt::AlignRight);
  uprightStabilityLayout->addWidget(saveEigFilenameEdit,1,0,1,2);
  
  uprightStabilityLayout->addWidget(new QLabel( tr("pitch guess (rad) (blank for default)") ),2,0);
  QLineEdit *pitchGuessEdit = new QLineEdit("0.0");
  pitchGuessEdit->setAlignment(Qt::AlignRight);
  uprightStabilityLayout->addWidget(pitchGuessEdit,2,1);
  
  uprightStabilityLayout->addWidget(new QLabel( tr("number of evaluation points") ),3,0);
  QLineEdit *nEvalPointsEdit = new QLineEdit("201");
  nEvalPointsEdit->setAlignment(Qt::AlignRight);
  uprightStabilityLayout->addWidget(nEvalPointsEdit,3,1);
  
  uprightStabilityLayout->addWidget(new QLabel( tr("initial speed v_i (m/s)") ), 4,0);
  QLineEdit *initSpeedEdit = new QLineEdit("0.0"); // THESE FIELDS NEED TO BE ACTUAL NUMBERS, NEED TO STORE DEFAULTS SOMEWHERE.
  initSpeedEdit->setAlignment(Qt::AlignRight);
  uprightStabilityLayout->addWidget(initSpeedEdit,4,1);
  
  uprightStabilityLayout->addWidget(new QLabel( tr("final speed v_f (m/s)") ), 5,0);
  QLineEdit *finalSpeedEdit = new QLineEdit("10.0");
  finalSpeedEdit->setAlignment(Qt::AlignRight);
  uprightStabilityLayout->addWidget(finalSpeedEdit,5,1);

//  evalOptions * opt = new evalOptions;
//  processOptions(argc, argv, opt, bb);
  /*
 //     MJWhippleParams * mjbike = new MJWhippleParams;
      WhippleParams * b = new WhippleParams;
      readMJWhippleParams(mjbike, optarg);
      convertParameters(b, mjbike);
      bike->setParameters(b);
      bike->evalConstants();
      bike->eoms();
      bike->computeOutputs();
      delete mjbike;
      delete b;
      
  // Vector to store range of speeds to calculate eigenvalues
  gsl_vector * speed = linspaceN(opt->vi, opt->vf, opt->N);

  bb->u1 = 0.0;
  bb->u3 = 0.0;
  bb->evalConstants();

  for (int i = 0; i < opt->N; ++i) {
    bb->u5 = -gsl_vector_get(speed, i)/(bb->rf+bb->rft);
    bb->calcEvals();
    OutputFile.write((char *) gsl_vector_ptr(speed, i), sizeof(double));
    OutputFile.write((char *) bb->fourValues, 4*sizeof(double));
  } // for i
  std::cout << "Eigenvalue data written to " << opt->outfolder << '\n';

  // Close files and free memory
  OutputFile.close();
  delete bb;
  delete opt;
  gsl_vector_free(speed);
  return 0;
 */ 
  
  
  uprightStabilityWidget->setLayout(uprightStabilityLayout);
}
void MainWindow::updateEigPlot(void)
{
	cout << "STOP PRESSING ME!" << endl;
	// do something
}

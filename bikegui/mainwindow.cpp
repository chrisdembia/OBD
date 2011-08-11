// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>

// OBD
#include "whipple.h"

// gui
#include "myqwhipple.h"
#include "mainwindow.h"
#include "parameters.h"
#include "myquprighttab.h"
#include "myqsteadytab.h"
#include "myqsimtab.h"

std::ostream &operator<<(std::ostream &outfile, const Whipple * discs);
// constructor
MainWindow::MainWindow() {
  // Set MainWindow parameterss
  setAnimated(true);
  setDockNestingEnabled(true);
  setWindowTitle(tr("Open Bicycle Dynamics"));

  // Allocate space for a Whipple object
  qbikes = new std::vector<MyQWhipple*>;

  // Set version string
  versionString = QString(tr("Version %1.%2 commit %3"))
              .arg(OBD_VERSION_MAJOR)
              .arg(OBD_VERSION_MINOR)
              .arg(OBD_VERSION_COMMIT);

  // Set up Actions, Menus, Tabs, Docks
  initActions();
  initMenus();
  //initDockWindows();
  initStatusBar();
  initTabs();

  //  QVBoxLayout 
  setCentralWidget(tabWidget);
} // MainWindow()

MainWindow::~MainWindow() {

} // ~MainWindow()

void MainWindow::about(void) {
  QString test = QString(tr("<h2>Open Bicycle Dynamics</h2>"
        "%1"
        "<p>Copyright &copy; 2010-2011 Dale Lukas Peterson</p>"
        "<p><a href='mailto:%2'>%2</a>"))
        .arg(versionString, OBD_AUTHOR_CONTACT);
  QMessageBox::about(this, tr("About OBD"), test);
} // about()

void MainWindow::initActions(void) {
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
} // initActions()

/*
void MainWindow::initDockWindows(void) {
  QDockWidget *dock = new QDockWidget(tr("Bike Definitions"));
  dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
  addDockWidget(Qt::LeftDockWidgetArea, dock);
  paramWidget = new WhippleParameter(qbikes, dock);
  dock->setWidget(paramWidget);
  
}*/

void MainWindow::initMenus(void) {
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
} // initMenus()

void MainWindow::initStatusBar(void) {
  statusBar()->showMessage(versionString);
} // initSatusBar()

void MainWindow::initTabs(void) {

//  msgLabel = new QLabel;

  tabWidget = new QTabWidget();
  uprightTab = new MyQUprightTab(qbikes, tabWidget);
  paramWidget = new WhippleParameter(qbikes, tabWidget);
  steadyTab = new MyQSteadyTab(qbikes, tabWidget);
  simTab = new MyQSimTab(qbikes,tabWidget);
  
  tabWidget->addTab( paramWidget, tr("Bicycle Library"));
  tabWidget->addTab( uprightTab, tr("Upright stability"));
  tabWidget->addTab( steadyTab, tr("Steady turning"));
  tabWidget->addTab( simTab, tr("Simulation"));
  
} // initTabs()

// VTK
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
  if (!chartRep) {
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
//         eigPlot->setMinimumSize(200, 200);
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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// cstd
#include <cstdlib>
#include <string>
#include <vector>
// qt
#include <QtGui>
#include <QVTKWidget.h>
// vtk
#include <vtkSmartPointer.h>
// vtk mappers
#include <vtkPolyDataMapper.h>
// vtk misc
#include <vtkTransform.h>
#include <vtkImageViewer.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
//#include <vtkJPEGReader.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
// obdgui
#include "parameters.h"
#include "mainwindow.h"
#include "myqwhipple.h"
#include "myqsimtab.h"
// obd
#include "whipple.h"
#include "whippleutils.h"
#include <getopt.h>
#include "gslVecUtils.h"
#include "OBDConfig.h"

class Whipple;
class QAction;
class WhippleParameter;
class Button;
// class QVTKWidget;

struct uprightOptions {
  std::string outfolder;
  double pitchguess;
  int N; // number of points on plot (linspace between velocities)
  double vi, vf; // initial and final velocity
} ;

class MainWindow : public QMainWindow
{
  Q_OBJECT;

public:
  MainWindow();
  // need the destructor too
  ~MainWindow();
  
private slots:
  void about(void);
  void updateEigPlotSlot(void);
  void saveEigSlot(void);

private:
  // Member functions
  void initActions(void);
  void initDockWindows(void);
  void initMenus(void);
  void initStatusBar(void);
  void initTabs(void);
  void initUprightStabilityTab(void);
  void initSteadyTurningTab(void);

  // Menus
  QMenu *fileMenu;
  QMenu *helpMenu;

  // Actions
  QAction *newAction;
  QAction *openAction;
  QAction *saveAction;
  QAction *saveAsAction;
  QAction *quitAction;
  QAction *aboutAction;
  QAction *aboutQtAction;

  // Version string
  QString versionString;

  // Tab widget
  QTabWidget *tabWidget;
  
  // Tabs
  QWidget *uprightStabilityTab;
  QWidget *steadyTurningTab;
  myQSimTab *simTab;
  
  // upright stability
  QLineEdit *saveEigFilenameEdit;
  QLineEdit *nEvalPointsEdit;
  QLineEdit *pitchGuessEdit;
  QLineEdit *initSpeedEdit;
  QLineEdit *finalSpeedEdit;

  QVTKWidget *eigPlotQVTKW; 

  // sim visualization

  // Parameter widget
  WhippleParameter *paramWidget;

  // A Whipple bike object
  Whipple *bike;
  uprightOptions upOpts;
};
#endif

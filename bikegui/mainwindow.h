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
#include "myquprighttab.h"
#include "myqsteadytab.h"
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


class MainWindow : public QMainWindow
{
  Q_OBJECT;

public:
  MainWindow();
  // need the destructor too
  ~MainWindow();

private slots:
  void about(void);

private:
  // Member functions
  void initActions(void);
  void initDockWindows(void);
  void initMenus(void);
  void initStatusBar(void);
  void initTabs(void);
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
  MyQUprightTab* uprightTab;
  MyQSteadyTab *steadyTab;
  myQSimTab *simTab;


  // sim visualization

  // Parameter widget
  WhippleParameter *paramWidget;

  // A Whipple bike object
  std::vector<MyQWhipple*> *qbikes;
  uprightOptions upOpts;
};
#endif

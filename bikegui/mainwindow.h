#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <vector>

#include <QtGui>
#include <QMainWindow>
#include <QVTKWidget.h>

#include <vtkSmartPointer.h>

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
  void simulateSlot(void);

private:
  // Member functions
  void createActions(void);
  void createDockWindows(void);
  void createMenus(void);
  void createStatusBar(void);
  void createTabs(void);
  void createUprightStabilityTab(void);
  void createSteadyTurningTab(void);
  void createMotionVisualizationTab(void);

  // motion visualization
  void drawTriad(void);

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

  // QVTK Widgets TODO
  //QVTKWidget *eigenPlot;
  //QVTKWidget *animationPlot;
  //QVTKWidget *eigenPlot;

  // Tab widget
  QTabWidget *tabWidget;
  
  // Tabs
  QWidget *uprightStabilityTab;
  QWidget *steadyTurningTab;
  QWidget *motionVisualizationTab;
  
  // upright stability
  QLineEdit *saveEigFilenameEdit;
  QLineEdit *nEvalPointsEdit;
  QLineEdit *pitchGuessEdit;
  QLineEdit *initSpeedEdit;
  QLineEdit *finalSpeedEdit;

  QVTKWidget *eigPlotQVTKW; 

  // motion visualization
  QVTKWidget *motionQVTKW; 
  vtkSmartPointer<vtkRenderer> motionRenderer;
  vtkSmartPointer<vtkRenderWindow> motionRenderWindow;
  // Parameter widget
  WhippleParameter *paramWidget;

  // A Whipple bike object
  Whipple *bike;
  uprightOptions upOpts;
};
#endif

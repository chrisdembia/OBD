#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "button.h"

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

private slots:
  void about(void);
  void updateEigPlot(void);

private:
  // Member functions
  void createActions(void);
  void createDockWindows(void);
  void createMenus(void);
  void createStatusBar(void);
  void createTabs(void);
  void createUprightStabilityTab(void);
  Button * createButton(const QString &text, const char *member);
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
  QWidget *uprightStabilityWidget;
  QWidget *steadyTurningWidget;
  QWidget *motionVisualizationWidget;
  
  // Parameter widget
  WhippleParameter *paramWidget;

  // A Whipple bike object
  Whipple *bike;
};
#endif

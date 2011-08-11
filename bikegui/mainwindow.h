#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>

// obdgui
class MyQWhipple;
class MainWindow;
class WhippleParameter;
#include "myquprighttab.h"
class MyQSteadyTab;
class myQSimTab;

// OBD
#include "whipple.h"

class MainWindow : public QMainWindow {
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




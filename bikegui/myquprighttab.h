#ifndef MYQUPRIGHT_H
#define MYQUPRIGHT_H

// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>
class QVTKWidget;

// vtk
#include <vtkSmartPointer.h>
class vtkFloatArray;
class vtkTable;
class vtkPlot;
class vtkChartXY;
class vtkContextView;
class vtkContextScene;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
// vtk saving plot
class vtkWindowToImageFilter;
class vtkPostScriptWriter;
class vtkPNGWriter;

// OBD
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"
#include "gslVecUtils.h"

// gui
class MyQWhipple;
class MyQUprightTab;

struct uprightOptions {
  double pitchguess;
  int N; // number of points on plot (linspace between velocities)
  double vi, vf; // first and last velocity
};

class MyQUprightTab : public QWidget {
  Q_OBJECT;

  public:
    MyQUprightTab(std::vector<MyQWhipple*>* qb, QWidget *parent = 0);
    //~MyQUprightTab;
    void calcIntersections();
// bike
  private slots:

    void updateEigPlotSlot(void);
    void saveEigPlotSlot(void);
    void saveEigDataSlot(void);

  private:
    std::vector<MyQWhipple*>* qbikes;
  // upOpts is a structure for run parameters for upright stability eigenvalue plots
  uprightOptions upOpts;
  
  QGridLayout *uprightSetLayout;
  
  // upright stability
  QLineEdit *nEvalPointsEdit;
  QLineEdit *pitchGuessEdit;
  QLineEdit *firstSpeedEdit;
  QLineEdit *lastSpeedEdit;

  QToolButton * saveEigPlotButton;
  QToolButton * saveEigDataButton;
  QToolButton * updateEigButton;

  QVTKWidget *eigPlotQVTKW; 

  QGroupBox *uprightSetBox;

  QHBoxLayout *uprightTopLayout;

  vtkSmartPointer<vtkContextView> eigPlotVTKView;
  vtkSmartPointer<vtkTable> eigPlotVTKTable;
  vtkSmartPointer<vtkChartXY> eigPlotVTKChart;
  vtkSmartPointer<vtkFloatArray> arrX;
  std::vector< vtkSmartPointer<vtkFloatArray> > arrY;

  vtkSmartPointer<vtkWindowToImageFilter> w2i;
  vtkSmartPointer<vtkPostScriptWriter> writerPS;
  vtkSmartPointer<vtkPNGWriter> writerPNG;
};
#endif

#ifndef MYQUPRIGHT_H
#define MYQUPRIGHT_H

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

// for saving the plot
#include <vtkPostScriptWriter.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include <getopt.h>
#include "gslVecUtils.h"
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"
#include "myqwhipple.h"

struct uprightOptions {
  std::string outfolder;
  double pitchguess;
  int N; // number of points on plot (linspace between velocities)
  double vi, vf; // first and last velocity
} ;

class MyQUprightTab : public QWidget
{
  Q_OBJECT;

  public:
    MyQUprightTab(std::vector<MyQWhipple*>* qb, QWidget *parent = 0);
    void calcIntersections();
// bike
  private slots:

    void updateEigPlotSlot(void);
    void saveEigSlot(void);

  private:
    std::vector<MyQWhipple*>* qbikes;
  // upOpts is a structure for run parameters for upright stability eigenvalue plots
  uprightOptions upOpts;
  
  QGridLayout *uprightSetLayout;
  
  // upright stability
  QLineEdit *saveEigFilenameEdit;
  QLineEdit *nEvalPointsEdit;
  QLineEdit *pitchGuessEdit;
  QLineEdit *firstSpeedEdit;
  QLineEdit *lastSpeedEdit;

  QToolButton * saveEigButton;
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

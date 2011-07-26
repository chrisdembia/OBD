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

#include <getopt.h>
#include "gslVecUtils.h"
#include "whipple.h"
#include "whippleutils.h"
#include "OBDConfig.h"


struct uprightOptions {
  std::string outfolder;
  double pitchguess;
  int N; // number of points on plot (linspace between velocities)
  double vi, vf; // initial and final velocity
} ;

class MyQUprightTab : public QWidget
{
  Q_OBJECT;

  public:
    MyQUprightTab(Whipple* bikes, QWidget *parent);
//    MyQWhipple* qbikes;
// bike
  private slots:

    void updateEigPlotSlot(void);
    void saveEigSlot(void);

  private:
    Whipple *bike;
  // upOpts is a structure for run parameters for upright stability eigenvalue plots
  uprightOptions upOpts;
  
  QGridLayout *uprightSetLayout;
  
  // upright stability
  QLineEdit *saveEigFilenameEdit;
  QLineEdit *nEvalPointsEdit;
  QLineEdit *pitchGuessEdit;
  QLineEdit *initSpeedEdit;
  QLineEdit *finalSpeedEdit;

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
};
#endif

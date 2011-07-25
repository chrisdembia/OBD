#ifndef MYQSIMTAB_H
#define MYQSIMTAB_H

#include <cstdlib>
#include <string>
#include <vector>

#include <QtGui>
#include <QVTKWidget.h>

// vtk sources
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkArrowSource.h>
#include <vtkPlaneSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
// vtk filters
#include <vtkTransformPolyDataFilter.h>
#include <vtkWindowToImageFilter.h>
// vtk mappers
#include <vtkTextMapper.h>
#include <vtkPolyDataMapper.h>
// vtk actors
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkAssembly.h>
// vtk misc
#include <vtkTransform.h>
#include <vtkImageViewer.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>
// vtk plotting
#include <vtkTable.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkChartXY.h>
//#include <vtkJPEGReader.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

#include "parameters.h"
#include "whipple.h"
#include "myvtkTriad.h"
#include "myqwhipple.h"

class myQSimTab : public QWidget
{
  Q_OBJECT;

  public:
  myQSimTab(Whipple* b, QWidget*parent);

  private slots:
  void startsimSlot(void);
  void updatePlotSlot(void);

  private:
  Whipple* bike;
  MyQWhipple* qbike1;

  QGroupBox *simLSetBox;
  QGridLayout *simLSetLayout;
//  QGroupBox* simRSetBox = new
	QGridLayout *simLayout;

  // simLSetBox
  QLabel *label1;
  QToolButton* startsimButton;

  // animation
  QVTKWidget *simQVTKW; 
  vtkSmartPointer<vtkRenderer> simRenderer;
  vtkSmartPointer<vtkRenderWindow> simRenderWindow;

  // plot
  QVTKWidget *simPlotQVTKW; 
  vtkSmartPointer<vtkContextView> simPlotVTKView; // This contains a chart object
  vtkSmartPointer<vtkChartXY> simPlotVTKChart;
  
};

#endif

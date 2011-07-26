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

class vtkTimerCallback2 : public vtkCommand
{
  public:
    static vtkTimerCallback2 *New()
    {
      vtkTimerCallback2 *cb = new vtkTimerCallback2;
      cb->TimerCount = 0;
      return cb;
    }
 
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void * vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent == eventId) {
        ++this->TimerCount;
      }
      std::cout << this->TimerCount << std::endl;
      // UPDATE
      time = (double)TimerCount/(double)qbike->GetBike()->fps; 
      while (qbike->GetBike()->t < time) {
        qbike->GetBike()->evolve(time,state);
      }

      qbike->MotionUpdate();
      qbike->MotionSetValues(TimerCount);
      if (TimerCount == 100) {
  simPlotQVTKW->GetInteractor()->Initialize();
  simPlotQVTKW->GetInteractor()->Start();
      plotrenwin->Render();
      }
      std::cout << "simtableval" << qbike->GetSimTable()->GetValue(TimerCount,1) << std::endl;
      // render
      vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
      iren->GetRenderWindow()->Render();
//  w2i->SetInput(iren->GetRenderWindow());
//  writer->SetInput(w2i->GetOutput());
//  writer->SetFileName(QString("cld72qtvtkbike" + QString("%1").arg(TimerCount) + ".ps").toStdString().c_str());
//  writer->Write();
    }
 
  public:
    MyQWhipple *qbike;
    void SetState(double s[10]) {
      for (int i = 0; i < 10; i++) {
        state[i] = s[i];
      }
    }
    vtkSmartPointer<vtkPostScriptWriter> writer;
    vtkSmartPointer<vtkWindowToImageFilter> w2i;
    vtkSmartPointer<vtkRenderWindow> plotrenwin;
    QVTKWidget* simPlotQVTKW;
  private:
    int TimerCount;
    double time;
    double state[10];
};

class myQSimTab : public QWidget
{
  Q_OBJECT;

  public:
  myQSimTab(std::vector<MyQWhipple*>* qb, Whipple* b, QWidget*parent);

  private slots:
  void startsimSlot(void);
  void stopsimSlot(void);
  void updatePlotSlot(void);

  private:
  Whipple* bike;
  MyQWhipple* qbike1;
  std::vector<MyQWhipple*>* qbikes;

  QGroupBox *simLSetBox;
  QGridLayout *simLSetLayout;
//  QGroupBox* simRSetBox = new
	QGridLayout *simLayout;

  // simLSetBox
  QLabel *label1;
  QToolButton* startsimButton;
  QToolButton* stopsimButton;

  // animation
  QVTKWidget *simQVTKW; 
  vtkSmartPointer<vtkRenderer> simRenderer;
  vtkSmartPointer<vtkRenderWindow> simRenderWindow;
  vtkSmartPointer<vtkTimerCallback2> simCallback;

  // plot
  QVTKWidget *simPlotQVTKW; 
  vtkSmartPointer<vtkContextView> simPlotVTKView; // This contains a chart object
  vtkSmartPointer<vtkChartXY> simPlotVTKChart;
  
};

#endif

#ifndef MYQSIMTAB_H
#define MYQSIMTAB_H

// c
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>
class QVTKWidget;

// vtk
#include <vtkSmartPointer.h>
class vtkActor;
class vtkActor2D;
class vtkAssembly;
class vtkCamera;
class vtkChartXY;
#include <vtkCommand.h>
//class vtkCommand;
class vtkContextView;
class vtkContextScene;
class vtkJPEGWriter;
class vtkPlaneSource;
class vtkPlot;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkPostScriptWriter;
class vtkProperty;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkTable;
class vtkTransform;
class vtkTextMapper;
class vtkTextProperty;
#include <vtkVariant.h>
//class vtkVariant;
class vtkWindowToImageFilter;

// OBD
#include "whipple.h"

// gui
#include "myqwhipple.h"
//class MyQWhipple;
class myvtkTriad;

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
      time = (double)TimerCount/(double)qbikes->at(0)->getBike()->fps; 
      while (qbikes->at(0)->getBike()->t < time) {
        if (TimerCount > 100 && TimerCount < 110) {
          qbikes->at(0)->getBike()->Ts = 5;
        }
        else { qbikes->at(0)->getBike()->Ts = 0; }
        qbikes->at(0)->getBike()->evolve(time,state);
      }
      qbikes->at(0)->SimUpdate();
      qbikes->at(0)->SetSimValues(TimerCount);
  //simPlotQVTKW->GetInteractor()->Initialize();
  //simPlotQVTKW->GetInteractor()->Start();
//  simPlotVTKChart->Paint(simPlotVTKView->GetContext());
  // simPlotVTKView->GetScene()->GetItem(0)->Paint(simPlotVTKView->GetContext());
  // simPlotVTKView->GetScene()->Paint(simPlotVTKView->GetContext());
   //   plotrenwin->Render();
      std::cout << "simtableval " <<
        qbikes->at(0)->GetSimTable()->GetValue(TimerCount,1) << std::endl;
      // render
      vtkRenderWindowInteractor *iren =
        vtkRenderWindowInteractor::SafeDownCast(caller);
    iren->GetRenderWindow()->Modified();
      iren->GetRenderWindow()->Render();
//  w2i->SetInput(iren->GetRenderWindow());
//  writer->SetInput(w2i->GetOutput());
//  writer->SetFileName(QString("cld72qtvtkbike" + QString("%1").arg(TimerCount) + ".ps").toStdString().c_str());
//  writer->Write();
    }
 
  public:
    std::vector<MyQWhipple*>* qbikes;
    void SetState(double s[10]) {
      for (int i = 0; i < 10; i++) {
        state[i] = s[i];
      }
    }
    vtkSmartPointer<vtkPostScriptWriter> writer;
    vtkSmartPointer<vtkWindowToImageFilter> w2i;
  //  vtkSmartPointer<vtkRenderWindow> plotrenwin;
   // QVTKWidget* simPlotQVTKW;
   // vtkSmartPointer<vtkChartXY> simPlotVTKChart;
   // vtkSmartPointer<vtkContextView> simPlotVTKView;
  private:
    int TimerCount;
    double time;
    double state[10];
};

class vtkTimerCallback3 : public vtkCommand
{
  public:
    static vtkTimerCallback3 *New()
    {
      vtkTimerCallback3 *cb = new vtkTimerCallback3;
      cb->TimerCount = 0;
      return cb;
    }
 
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void * vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent == eventId) {
        ++this->TimerCount;
      }
      std::cout << "count3 " << this->TimerCount << std::endl;
      // UPDATE
//      qbikes->at(0)->SimSetValues(TimerCount);
  //simPlotQVTKW->GetInteractor()->Initialize();
  //simPlotQVTKW->GetInteractor()->Start();
      //plotrenwin->Render();
      vtkRenderWindowInteractor *iren =
        vtkRenderWindowInteractor::SafeDownCast(caller);
      iren->GetRenderWindow()->Render();
      // render
    }
 
  public:
    std::vector<MyQWhipple*>* qbikes;
    vtkSmartPointer<vtkRenderWindow> plotrenwin;
    QVTKWidget* simPlotQVTKW;
  private:
    int TimerCount;
    double time;
};

class myQSimTab : public QWidget
{
  Q_OBJECT;

  public:
  myQSimTab(std::vector<MyQWhipple*>* qb, QWidget*parent = 0);

  private slots:
  void startsimSlot(void);
  void stopsimSlot(void);
  void updatePlotSlot(void);
  void forceCheckSlot(int);
  void writeSimSlot(void);
  void saveSimagesSlot(void);

  private:
  std::vector<MyQWhipple*>* qbikes;

  QGroupBox *simLSetBox;
  QGridLayout *simLSetLayout;
//  QGroupBox* simRSetBox = new
	QGridLayout *simLayout;

  // simLSetBox
  QLabel *speedLabel;
  QLineEdit *speedEdit;
  QToolButton* startsimButton;
  QToolButton* stopsimButton;
  QCheckBox* forceCheck;
  QToolButton* writesimButton;
  QToolButton* savesimagesButton;

  // animation
  QVTKWidget *simQVTKW; 
  vtkSmartPointer<vtkRenderer> simRenderer;
  vtkSmartPointer<vtkRenderWindow> simRenderWindow;
  vtkSmartPointer<vtkTimerCallback2> simCallback;
  vtkSmartPointer<vtkTimerCallback3> simPlotCallback;

  // plot
  QVTKWidget *simPlotQVTKW; 
  vtkSmartPointer<vtkContextView> simPlotVTKView; 
  vtkSmartPointer<vtkChartXY> simPlotVTKChart;
//  std::vector<vtkSmartPointer<vtkPlot> > simPlotVTKLines;
  
};

#endif

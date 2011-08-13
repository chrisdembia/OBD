#ifndef MYVTKBIKE_H
#define MYVTKBIKE_H

// c
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>

// qt
#include <QtGui>

// vtk
#include <vtkSmartPointer.h>
class vtkActor;
class vtkAssembly;
class vtkCamera;
class vtkCellArray;
#include <vtkCommand.h>
class vtkConeSource;
class vtkContext2D;
class vtkCylinderSource;
class vtkFloatArray;
class vtkJPEGWriter;
class vtkParametricFunctionSource;
class vtkParametricTorus;
class vtkPen;
class vtkPoints;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkPolyLine;
class vtkPostScriptWriter;
class vtkProperty;
class vtkRenderer;
//class vtkRenderWindow;
#include <vtkRenderWindow.h>
//class vtkRenderWindowInteractor;
#include <vtkRenderWindowInteractor.h>
class vtkSphereSource;
//class vtkTable;
#include <vtkTable.h>
class vtkTransform;
class vtkTransformPolyDataFilter;
class vtkVariant;
class vtkWindowToImageFilter;

// OBD
#include "whipple.h"

// gui
class myquprighttab;
class myvtkTriad;

class MyQWhipple {
  public:
    MyQWhipple(std::string);
    ~MyQWhipple();
    void SimUpdate();
    Whipple* getBike();
    std::string getName();

    // upright
    void initUprightTable();
    void calcUpright(struct uprightOptions upOpts);
    vtkSmartPointer<vtkTable> GetUprightTable();

    // sim
    vtkSmartPointer<vtkTable> GetSimTable();
    void SetSimValues(int rowidx);
    void initSim(vtkSmartPointer<vtkRenderer> ren);
    void writeSim(std::string);

    void Draw2D(vtkSmartPointer<vtkContext2D>);
    void QDraw2D(QGraphicsScene* qscene);
    void UpdateTrace();
    void TurnOnReactionTriads();
    void TurnOffReactionTriads();

    void setGyroParams( WhippleParams* b);
    void setMeijParams( MJWhippleParams* b);
    WhippleParams* getGyroParams();
    MJWhippleParams* getMeijParams();

    void setParamType(int p);
    int getParamType();

    QString gyroFileLabelText;
    QString meijFileLabelText;

    void setDoUpright(bool);
    bool getDoUpright();
    
    void printUprightData(QString fname);

  private:
    std::string name;
    vtkSmartPointer<vtkRenderer> simRenderer;
    Whipple* bike;
    int paramtype;
    WhippleParams* gswp;
    MJWhippleParams* mjwp;

    // UPRIGHT
    bool doUpright;
    vtkSmartPointer<vtkTable> eigPlotVTKTable;
    vtkSmartPointer<vtkFloatArray> eigX;
    std::vector< vtkSmartPointer<vtkFloatArray> > eigY;
     
    
    // SIM
    // rear wheel: two cones and a torus
    // rear cone right
    // source
    vtkSmartPointer<vtkConeSource> rearConeRightSource;
    // transform
    vtkSmartPointer<vtkTransform> rearConeRightTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> rearConeRightFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> rearConeRightMapper;
    // actor
    vtkSmartPointer<vtkActor> rearConeRightActor;
  
    // rear cone left
    // source
    vtkSmartPointer<vtkConeSource> rearConeLeftSource;
    // transform
    vtkSmartPointer<vtkTransform> rearConeLeftTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> rearConeLeftFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> rearConeLeftMapper;
    // actor
    vtkSmartPointer<vtkActor> rearConeLeftActor;
    
    // rear torus
    vtkSmartPointer<vtkParametricTorus> rearTorus;
    // source
    vtkSmartPointer<vtkParametricFunctionSource> rearTorusSource;
    // transform
    vtkSmartPointer<vtkTransform> rearTorusTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> rearTorusFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> rearTorusMapper;
    // actor
    vtkSmartPointer<vtkActor> rearTorusActor;
  
    // rear frame: center of mass ball and tube
    // rear center of mass ball
    // source
    vtkSmartPointer<vtkSphereSource> rearCOMSource;
    // transform
    vtkSmartPointer<vtkTransform> rearCOMTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> rearCOMFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> rearCOMMapper;
    // actor
    vtkSmartPointer<vtkActor> rearCOMActor;
    
    // rear frame tube
    // source
    vtkSmartPointer<vtkCylinderSource> rearTubeSource;
    // transform
    vtkSmartPointer<vtkTransform> rearTubeTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> rearTubeFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> rearTubeMapper;
    // actor
    vtkSmartPointer<vtkActor> rearTubeActor;
  
    // front frame
    // front frame tube
    // source
    vtkSmartPointer<vtkCylinderSource> frontTubeSource;
    // transform
    vtkSmartPointer<vtkTransform> frontTubeTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> frontTubeFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> frontTubeMapper;
    // actor
    vtkSmartPointer<vtkActor> frontTubeActor;
  
    // front frame offset
    // source
    vtkSmartPointer<vtkCylinderSource> frontOffsetSource;
    // transform
    vtkSmartPointer<vtkTransform> frontOffsetTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> frontOffsetFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> frontOffsetMapper;
    // actor
    vtkSmartPointer<vtkActor> frontOffsetActor;
  
    // front wheel: two cones and a torus
    // front cone right
    // source
    vtkSmartPointer<vtkConeSource> frontConeRightSource;
    // transform
    vtkSmartPointer<vtkTransform> frontConeRightTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> frontConeRightFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> frontConeRightMapper;
    // actor
    vtkSmartPointer<vtkActor> frontConeRightActor;
  
    // front cone left
    // source
    vtkSmartPointer<vtkConeSource> frontConeLeftSource;
    // transform
    vtkSmartPointer<vtkTransform> frontConeLeftTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> frontConeLeftFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> frontConeLeftMapper;
    // actor
    vtkSmartPointer<vtkActor> frontConeLeftActor;
    
    // front torus
    vtkSmartPointer<vtkParametricTorus> frontTorus;
    // source
    vtkSmartPointer<vtkParametricFunctionSource> frontTorusSource;
    // transform
    vtkSmartPointer<vtkTransform> frontTorusTransform;
    // filter
    vtkSmartPointer<vtkTransformPolyDataFilter> frontTorusFilter;
    // mapper
    vtkSmartPointer<vtkPolyDataMapper> frontTorusMapper;
    // actor
    vtkSmartPointer<vtkActor> frontTorusActor;
    
    // vector triads
    myvtkTriad *triad0;
    myvtkTriad *triad1;
    myvtkTriad *triad2;
    myvtkTriad *triad3;
    myvtkTriad *triad4;
 
    // ground reaction force triads
    myvtkTriad *rearReaction;

    // render the actor assemblies
    vtkSmartPointer<vtkAssembly> rearWheelAssy;
    vtkSmartPointer<vtkAssembly> rearFrameAssy;
    vtkSmartPointer<vtkAssembly> frontFrameAssy;
    vtkSmartPointer<vtkTransform> frontFrameTransform;
    vtkSmartPointer<vtkAssembly> frontWheelAssy;
    vtkSmartPointer<vtkTransform> frontWheelTransform;
    
    // wheel traces
    vtkSmartPointer<vtkPoints> rearPoints;
    vtkSmartPointer<vtkPolyLine> rearPointsLine;
    vtkSmartPointer<vtkCellArray> rearPointsCell;
    vtkSmartPointer<vtkPolyData> rearPointsData;
    vtkSmartPointer<vtkPolyDataMapper> rearPointsMapper;
    vtkSmartPointer<vtkActor> rearPointsActor;
  
    vtkSmartPointer<vtkPoints> frontPoints;
    vtkSmartPointer<vtkPolyLine> frontPointsLine;
    vtkSmartPointer<vtkCellArray> frontPointsCell;
    vtkSmartPointer<vtkPolyData> frontPointsData;
    vtkSmartPointer<vtkPolyDataMapper> frontPointsMapper;
    vtkSmartPointer<vtkActor> frontPointsActor;
  
    // sim data
    std::vector<std::string> simVarNames;
    vtkSmartPointer<vtkTable> simTable;
    std::vector<vtkSmartPointer<vtkFloatArray> > simArrays;
};

#endif

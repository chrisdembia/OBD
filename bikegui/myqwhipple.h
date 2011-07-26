#ifndef MYVTKBIKE_H
#define MYVTKBIKE_H
#include <cstdlib>
#include <string>
#include <vector>

// vtk sources
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkArrowSource.h>
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
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
//#include <vtkJPEGReader.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>
// vtk data
#include <vtkTable.h>
#include <vtkFloatArray.h>

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


#include <vtkPlaneSource.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
// vtk filters
#include <vtkTransformPolyDataFilter.h>
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
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>
//#include <vtkJPEGReader.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkPolyData.h>
#include <vtkCommand.h>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#include "whipple.h"
#include "myvtkTriad.h"


class MyQWhipple
{
  public:
  MyQWhipple(std::string);
  ~MyQWhipple();
  void SimUpdate();
  Whipple* getBike();
  vtkSmartPointer<vtkTable> GetSimTable();
  void SimSetValues(int rowidx);
  void initSim(vtkSmartPointer<vtkRenderer> ren);

  private:
  std::string name;
  vtkSmartPointer<vtkRenderer> simRenderer;
  Whipple* bike;
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
  
  // render the actor assemblies
  vtkSmartPointer<vtkAssembly> rearWheelAssy;
  vtkSmartPointer<vtkAssembly> rearFrameAssy;
  vtkSmartPointer<vtkAssembly> frontFrameAssy;
  vtkSmartPointer<vtkTransform> frontFrameTransform;
  vtkSmartPointer<vtkAssembly> frontWheelAssy;
  vtkSmartPointer<vtkTransform> frontWheelTransform;
  
  // sim data
  std::vector<std::string> simVarNames;
  vtkSmartPointer<vtkTable> simTable;
  std::vector<vtkSmartPointer<vtkFloatArray> > simArrays;
};

#endif

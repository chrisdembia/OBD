#include <cstdlib>
#include <string>
#include <vector>


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
// vtk data
#include <vtkTable.h>
#include <vtkFloatArray.h>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPostScriptWriter.h>

#include "mainwindow.h"
#include "myvtkTriad.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()
#define NMOTIONVARS 32

MyQWhipple::MyQWhipple(vtkSmartPointer<vtkRenderer> ren,Whipple* b)
{
  renderer = ren;
  bike = b;
  // rear wheel: two cones and a torus
  double hubhalfwidth = .1;
  // rear cone right
  // source
  rearConeRightSource = vtkConeSource::New();
  rearConeRightSource->SetHeight(hubhalfwidth);
  rearConeRightSource->SetRadius(bike->rr);
  rearConeRightSource->SetResolution(100);
  rearConeRightSource->Update();
  // transform
  rearConeRightTransform = vtkTransform::New();
  rearConeRightTransform->Identity();
  rearConeRightTransform->Translate(0,hubhalfwidth/2,0);
  rearConeRightTransform->RotateZ(90);
  // filter
  rearConeRightFilter = vtkTransformPolyDataFilter::New();
  rearConeRightFilter->SetInputConnection(rearConeRightSource->GetOutputPort());
  rearConeRightFilter->SetTransform(rearConeRightTransform);
  // mapper
  rearConeRightMapper = vtkPolyDataMapper::New();
  rearConeRightMapper->SetInputConnection(rearConeRightFilter->GetOutputPort());
  // actor
  rearConeRightActor = vtkActor::New();
  rearConeRightActor->SetMapper(rearConeRightMapper);
  rearConeRightActor->GetProperty()->SetOpacity(0.7);

  // rear cone left
  // source
  rearConeLeftSource = vtkConeSource::New();
  rearConeLeftSource->SetHeight(hubhalfwidth);
  rearConeLeftSource->SetRadius(bike->rr);
  rearConeLeftSource->SetResolution(100);
  rearConeLeftSource->Update();
  // transform
  rearConeLeftTransform = vtkTransform::New();
  rearConeLeftTransform->Identity();
  rearConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  rearConeLeftTransform->RotateZ(-90);
  // filter
  rearConeLeftFilter = vtkTransformPolyDataFilter::New();
  rearConeLeftFilter->SetInputConnection(rearConeLeftSource->GetOutputPort());
  rearConeLeftFilter->SetTransform(rearConeLeftTransform);
  // mapper
  rearConeLeftMapper = vtkPolyDataMapper::New();
  rearConeLeftMapper->SetInputConnection(rearConeLeftFilter->GetOutputPort());
  // actor
  rearConeLeftActor = vtkActor::New();
  rearConeLeftActor->SetMapper(rearConeLeftMapper);
  rearConeLeftActor->GetProperty()->SetOpacity(0.7);
  
  // rear torus
  rearTorus = vtkParametricTorus::New();
  // source
  rearTorusSource = vtkParametricFunctionSource::New();
  rearTorusSource->SetParametricFunction(rearTorus);
  rearTorus->SetRingRadius(bike->rr);
  rearTorus->SetCrossSectionRadius(bike->rrt);
  rearTorusSource->Update();
  // transform
  rearTorusTransform = vtkTransform::New();
  rearTorusTransform->Identity();
  rearTorusTransform->RotateX(90);
  // filter
  rearTorusFilter = vtkTransformPolyDataFilter::New();
  rearTorusFilter->SetInputConnection(rearTorusSource->GetOutputPort());
  rearTorusFilter->SetTransform(rearTorusTransform);
  // mapper
  rearTorusMapper = vtkPolyDataMapper::New();
  rearTorusMapper->SetInputConnection(rearTorusFilter->GetOutputPort());
  // actor
  rearTorusActor = vtkActor::New();
  rearTorusActor->SetMapper(rearTorusMapper);

  // rear frame: center of mass ball and tube
  // rear center of mass ball
  // source
  rearCOMSource = vtkSphereSource::New();
  rearCOMSource->SetRadius(0.08);
  rearCOMSource->SetThetaResolution(100);
  rearCOMSource->SetPhiResolution(100);
  rearCOMSource->Update();
  // transform
  rearCOMTransform = vtkTransform::New();
  rearCOMTransform->Identity();
  rearCOMTransform->Translate(bike->lrx,0,bike->lrz);
  // filter
  rearCOMFilter = vtkTransformPolyDataFilter::New();
  rearCOMFilter->SetInputConnection(rearCOMSource->GetOutputPort());
  rearCOMFilter->SetTransform(rearCOMTransform);
  // mapper
  rearCOMMapper = vtkPolyDataMapper::New();
  rearCOMMapper->SetInputConnection(rearCOMFilter->GetOutputPort());
  // actor
  rearCOMActor = vtkActor::New();
  rearCOMActor->SetMapper(rearCOMMapper);
  
  // rear frame tube
  // source
  rearTubeSource = vtkCylinderSource::New();
  rearTubeSource->SetRadius(0.012);
  rearTubeSource->SetHeight(bike->lr);
  // transform
  rearTubeTransform = vtkTransform::New();
  rearTubeTransform->Identity();
  rearTubeTransform->RotateZ(90);
  rearTubeTransform->Translate(0,-bike->lr/2,0);
  // filter
  rearTubeFilter = vtkTransformPolyDataFilter::New();
  rearTubeFilter->SetInputConnection(rearTubeSource->GetOutputPort());
  rearTubeFilter->SetTransform(rearTubeTransform);
  // mapper
  rearTubeMapper = vtkPolyDataMapper::New();
  rearTubeMapper->SetInputConnection(rearTubeFilter->GetOutputPort());
  // actor
  rearTubeActor = vtkActor::New();
  rearTubeActor->SetMapper(rearTubeMapper);

  // front frame
  // front frame tube
  // source
  frontTubeSource = vtkCylinderSource::New();
  frontTubeSource->SetRadius(0.012);
  frontTubeSource->SetHeight(bike->ls);
  // transform
  frontTubeTransform = vtkTransform::New();
  frontTubeTransform->Identity();
  frontTubeTransform->RotateZ(90);
  frontTubeTransform->Translate(0,-bike->ls/2,0);
  // filter
  frontTubeFilter = vtkTransformPolyDataFilter::New();
  frontTubeFilter->SetInputConnection(frontTubeSource->GetOutputPort());
  frontTubeFilter->SetTransform(frontTubeTransform);
  // mapper
  frontTubeMapper = vtkPolyDataMapper::New();
  frontTubeMapper->SetInputConnection(frontTubeFilter->GetOutputPort());
  // actor
  frontTubeActor = vtkActor::New();
  frontTubeActor->SetMapper(frontTubeMapper);

  // front frame offset
  // source
  frontOffsetSource = vtkCylinderSource::New();
  frontOffsetSource->SetRadius(0.012);
  frontOffsetSource->SetHeight(bike->lf);
//  frontOffsetSource->DebugOn();
  // transform
  frontOffsetTransform = vtkTransform::New();
  frontOffsetTransform->Identity();
  frontOffsetTransform->RotateX(90);
  frontOffsetTransform->Translate(bike->ls,-bike->lf/2,0);
  // filter
  frontOffsetFilter = vtkTransformPolyDataFilter::New();
  frontOffsetFilter->SetInputConnection(frontOffsetSource->GetOutputPort());
  frontOffsetFilter->SetTransform(frontOffsetTransform);
  // mapper
  frontOffsetMapper = vtkPolyDataMapper::New();
  frontOffsetMapper->SetInputConnection(frontOffsetFilter->GetOutputPort());
  // actor
  frontOffsetActor = vtkActor::New();
  frontOffsetActor->SetMapper(frontOffsetMapper);

  // front wheel: two cones and a torus
  // front cone right
  // source
  frontConeRightSource = vtkConeSource::New();
  frontConeRightSource->SetHeight(hubhalfwidth);
  frontConeRightSource->SetRadius(bike->rr);
  frontConeRightSource->SetResolution(100);
  frontConeRightSource->Update();
  // transform
  frontConeRightTransform = vtkTransform::New();
  frontConeRightTransform->Identity();
  frontConeRightTransform->Translate(0,hubhalfwidth/2,0);
  frontConeRightTransform->RotateZ(90);
  // filter
  frontConeRightFilter = vtkTransformPolyDataFilter::New();
  frontConeRightFilter->SetInputConnection(frontConeRightSource->GetOutputPort());
  frontConeRightFilter->SetTransform(frontConeRightTransform);
  // mapper
  frontConeRightMapper = vtkPolyDataMapper::New();
  frontConeRightMapper->SetInputConnection(frontConeRightFilter->GetOutputPort());
  // actor
  frontConeRightActor = vtkActor::New();
  frontConeRightActor->SetMapper(frontConeRightMapper);
  frontConeRightActor->GetProperty()->SetOpacity(0.7);

  // front cone left
  // source
  frontConeLeftSource = vtkConeSource::New();
  frontConeLeftSource->SetHeight(hubhalfwidth);
  frontConeLeftSource->SetRadius(bike->rr);
  frontConeLeftSource->SetResolution(100);
  frontConeLeftSource->Update();
  // transform
  frontConeLeftTransform = vtkTransform::New();
  frontConeLeftTransform->Identity();
  frontConeLeftTransform->Translate(0,-hubhalfwidth/2,0);
  frontConeLeftTransform->RotateZ(-90);
  // filter
  frontConeLeftFilter = vtkTransformPolyDataFilter::New();
  frontConeLeftFilter->SetInputConnection(frontConeLeftSource->GetOutputPort());
  frontConeLeftFilter->SetTransform(frontConeLeftTransform);
  // mapper
  frontConeLeftMapper = vtkPolyDataMapper::New();
  frontConeLeftMapper->SetInputConnection(frontConeLeftFilter->GetOutputPort());
  // actor
  frontConeLeftActor = vtkActor::New();
  frontConeLeftActor->SetMapper(frontConeLeftMapper);
  frontConeLeftActor->GetProperty()->SetOpacity(0.7);
  
  // front torus
  frontTorus = vtkParametricTorus::New();
  // source
  frontTorusSource = vtkParametricFunctionSource::New();
  frontTorusSource->SetParametricFunction(frontTorus);
  frontTorus->SetRingRadius(bike->rr);
  frontTorus->SetCrossSectionRadius(bike->rrt);
  frontTorusSource->Update();
  // transform
  frontTorusTransform = vtkTransform::New();
  frontTorusTransform->Identity();
  frontTorusTransform->RotateX(90);
  // filter
  frontTorusFilter = vtkTransformPolyDataFilter::New();
  frontTorusFilter->SetInputConnection(frontTorusSource->GetOutputPort());
  frontTorusFilter->SetTransform(frontTorusTransform);
  // mapper
  frontTorusMapper = vtkPolyDataMapper::New();
  frontTorusMapper->SetInputConnection(frontTorusFilter->GetOutputPort());
  // actor
  frontTorusActor = vtkActor::New();
  frontTorusActor->SetMapper(frontTorusMapper);

  // ASSEMBLIES
  // render the actor assemblies
  // rear wheel assembly
  rearWheelAssy = vtkAssembly::New();
  rearWheelAssy->AddPart(rearConeRightActor);
  rearWheelAssy->AddPart(rearConeLeftActor);
  rearWheelAssy->AddPart(rearTorusActor);

  // rear frame assembly
  rearFrameAssy = vtkAssembly::New();
  rearFrameAssy->AddPart(rearCOMActor);
  rearFrameAssy->AddPart(rearTubeActor);

  // front frame assembly
  frontFrameAssy = vtkAssembly::New();
  frontFrameTransform = vtkTransform::New();
  frontFrameAssy->AddPart(frontTubeActor);
  frontFrameAssy->AddPart(frontOffsetActor);

  // front wheel assembly
  frontWheelAssy = vtkAssembly::New();
  frontWheelTransform = vtkTransform::New();
  frontWheelAssy->AddPart(frontConeRightActor);
  frontWheelAssy->AddPart(frontConeLeftActor);

  // vector coordinate system triads
  triad0 = new myvtkTriad(renderer);
  triad0->SetScale(.4,.4,.4);
  triad1 = new myvtkTriad(renderer);
  triad1->SetScale(.2,.2,.2);
  triad2 = new myvtkTriad(renderer);
  triad2->SetScale(.2,.2,.2);
  triad3 = new myvtkTriad(renderer);
  triad3->SetScale(.2,.2,.2);
  triad4 = new myvtkTriad(renderer);
  triad4->SetScale(.2,.2,.2);

  // render
  renderer->AddActor(rearWheelAssy);
  renderer->AddActor(rearFrameAssy);
  renderer->AddActor(frontFrameAssy);
  renderer->AddActor(frontWheelAssy);

  // motion data storage
  motionVarNames.resize(NMOTIONVARS);
  motionVarNames[0] = "t";
  motionVarNames[1] = "q0";
  motionVarNames[2] = "q1";
  motionVarNames[3] = "q2";
  motionVarNames[4] = "q3";
  motionVarNames[5] = "q4";
  motionVarNames[6] = "q5";
  motionVarNames[7] = "q6";
  motionVarNames[8] = "q7";
  motionVarNames[9] = "u0";
  motionVarNames[10] = "u1";
  motionVarNames[11] = "u2";
  motionVarNames[12] = "u3";
  motionVarNames[13] = "u4";
  motionVarNames[14] = "u5";
  motionVarNames[15] = "fnx";
  motionVarNames[16] = "fny";
  motionVarNames[17] = "fnz";
  motionVarNames[18] = "Rx";
  motionVarNames[19] = "Ry";
  motionVarNames[20] = "Rz";
  motionVarNames[21] = "Fx";
  motionVarNames[22] = "Fy";
  motionVarNames[23] = "Fz";
  motionVarNames[24] = "KE";
  motionVarNames[25] = "PE";
  motionVarNames[26] = "fa_yaw";
  motionVarNames[27] = "fa_lean";
  motionVarNames[28] = "fa_pitch";
  motionVarNames[29] = "nh0";
  motionVarNames[30] = "nh1";
  motionVarNames[31] = "nh2";
  motionTable = vtkSmartPointer<vtkTable>::New();
  motionArrays.resize(NMOTIONVARS);

  for (uint i = 0; i < motionArrays.size(); i++) {
    motionArrays[i] = vtkSmartPointer<vtkFloatArray>::New();
    motionArrays[i]->SetName(motionVarNames[i].c_str());
    motionTable->AddColumn(motionArrays[i]);
  }
}
  
MyQWhipple::~MyQWhipple()
{
    delete triad1;
    delete triad2;
    delete triad3;
    delete triad4;
  }

Whipple* MyQWhipple::GetBike()
{
  return bike;
}

void MyQWhipple::MotionUpdate()
{
  double YAW = bike->q0;
  double LEAN = bike->q1;
  double PITCH = bike->q2;
  double STEER = bike->q3;
  double RWSPIN = bike->q4; 
  double FWSPIN = bike->q5;
  double X = bike->q6;
  double Y = bike->q7;
  // ASSEMBLIES
  // rear wheel assembly
  rearWheelAssy->SetPosition(-bike->rr*sin(YAW)*sin(LEAN) + X,
                             bike->rr*cos(YAW)*sin(LEAN) + Y,
                             -bike->rrt - bike->rr*cos(LEAN));
  rearWheelAssy->SetOrientation(0,0,0);
  rearWheelAssy->RotateZ(180/M_PI*YAW);// NEGATIVE? IS RIGHTWARD AS IS
  rearWheelAssy->RotateX(180/M_PI*LEAN);
  rearWheelAssy->RotateY(180/M_PI*RWSPIN);
  
  triad0->SetPosition(X,Y,0);
//  triad0->SetOrientation(rearFrameAssy->GetOrientation());

  triad1->SetPosition(rearWheelAssy->GetPosition());
  triad1->SetOrientation(rearWheelAssy->GetOrientation());

  // rear frame assembly
  rearFrameAssy->SetPosition(rearWheelAssy->GetPosition());
  rearFrameAssy->SetOrientation(rearWheelAssy->GetOrientation());
  rearFrameAssy->RotateY(180/M_PI*(-RWSPIN+PITCH));

  triad2->SetPosition(rearFrameAssy->GetPosition());
  triad2->SetOrientation(rearFrameAssy->GetOrientation());

  // front frame assembly
  frontFrameAssy->SetPosition(0,0,-1);

  // front frame transform
  frontFrameTransform->SetMatrix(rearFrameAssy->GetMatrix());
  frontFrameTransform->Translate(bike->lr,0,0);
  frontFrameTransform->RotateY(-90);
  frontFrameTransform->RotateX(-STEER);
  frontFrameAssy->SetPosition(frontFrameTransform->GetPosition());
  frontFrameAssy->SetOrientation(frontFrameTransform->GetOrientation());

  triad3->SetPosition(frontFrameAssy->GetPosition());
  triad3->SetOrientation(frontFrameAssy->GetOrientation());

  // front wheel assembly
  frontWheelAssy->SetPosition(0,0,-bike->rf - bike->rft);
  
  // front frame transform
  frontWheelTransform->SetMatrix(frontFrameAssy->GetMatrix());
  frontWheelTransform->Translate(bike->ls,0,-bike->lf);
  frontWheelTransform->RotateY(90-180/M_PI*(PITCH-FWSPIN));
  frontWheelAssy->SetPosition(frontWheelTransform->GetPosition());
  frontWheelAssy->SetOrientation(frontWheelTransform->GetOrientation());

  triad4->SetPosition(frontWheelAssy->GetPosition());
  triad4->SetOrientation(frontWheelAssy->GetOrientation());
//      iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
//    renderer->ResetCameraClippingRange();
//
  double d = 1;
  double pos[3];
  pos[0] = frontFrameAssy->GetPosition()[0];
  pos[1] = frontFrameAssy->GetPosition()[1];
  pos[2] = frontFrameAssy->GetPosition()[2];
//  double bounds[6] = frontFrameAssy->GetBounds();
  renderer->ResetCamera(pos[0]-d,pos[0]+d,
      pos[1]-d,pos[1]+d,
      pos[2]-d,pos[2]+d);
//  renderer->GetActiveCamera()->Dolly(.5);

 // renderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
  //frontFrameAssy->AddPosition(0,5,-2);
  //renderer->GetActiveCamera()->SetPosition(frontFrameAssy->GetPosition());
//  renderer->GetActiveCamera()->SetDistance(5);
//  renderer->GetActiveCamera()->SetViewUp(0,0,-1);
//  motionRenderer->GetActiveCamera()->Elevation(-95);
//  renderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
}

void MyQWhipple::MotionSetValues(int rowidx)
{
   // do some nice memory managemen here to control the size of the array
  for (uint i = 0; i < motionArrays.size(); i++) {
    motionTable->SetValue(rowidx,i,bike->t);
    motionTable->SetValue(rowidx,i,bike->q0);
    motionTable->SetValue(rowidx,i,bike->q1);
    motionTable->SetValue(rowidx,i,bike->q2);
    motionTable->SetValue(rowidx,i,bike->q3);
    motionTable->SetValue(rowidx,i,bike->q4);
    motionTable->SetValue(rowidx,i,bike->q5);
    motionTable->SetValue(rowidx,i,bike->q6);
    motionTable->SetValue(rowidx,i,bike->q7);
    motionTable->SetValue(rowidx,i,bike->u0);
    motionTable->SetValue(rowidx,i,bike->u1);
    motionTable->SetValue(rowidx,i,bike->u2);
    motionTable->SetValue(rowidx,i,bike->u3);
    motionTable->SetValue(rowidx,i,bike->u4);
    motionTable->SetValue(rowidx,i,bike->u5);
    motionTable->SetValue(rowidx,i,bike->no_fn[0]);
    motionTable->SetValue(rowidx,i,bike->no_fn[1]);
    motionTable->SetValue(rowidx,i,bike->no_fn[2]);
    motionTable->SetValue(rowidx,i,bike->Rx);
    motionTable->SetValue(rowidx,i,bike->Ry);
    motionTable->SetValue(rowidx,i,bike->Rz);
    motionTable->SetValue(rowidx,i,bike->Fx);
    motionTable->SetValue(rowidx,i,bike->Fy);
    motionTable->SetValue(rowidx,i,bike->Fz);
    motionTable->SetValue(rowidx,i,bike->ke);
    motionTable->SetValue(rowidx,i,bike->pe);
    motionTable->SetValue(rowidx,i,bike->fa_yaw);
    motionTable->SetValue(rowidx,i,bike->fa_lean);
    motionTable->SetValue(rowidx,i,bike->fa_pitch);
    motionTable->SetValue(rowidx,i,bike->constraints[0]);
    motionTable->SetValue(rowidx,i,bike->constraints[1]);
    motionTable->SetValue(rowidx,i,bike->constraints[2]);
  }
}


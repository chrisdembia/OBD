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
#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkConeSource.h>
#include <vtkContext2D.h>
#include <vtkCylinderSource.h>
#include <vtkFloatArray.h>
#include <vtkJPEGWriter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
#include <vtkPen.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyLine.h>
#include <vtkPostScriptWriter.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkTable.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkWindowToImageFilter.h>

// OBD
#include "whippleutils.h"

// gui
#include "myquprighttab.h"
#include "myqwhipple.h"
#include "myvtkTriad.h"

// define
#define NMOTIONVARS 32

MyQWhipple::MyQWhipple(std::string n) {
  name = n;
  bike = new Whipple();
  paramtype = 0; // gyrostat
  gswp = new WhippleParams;
  mjwp = new MJWhippleParams;
  setBenchmarkParameters(mjwp);
  convertParameters( gswp, mjwp);

  doUpright = true;
  initUprightTable();

}
  
MyQWhipple::~MyQWhipple() {
    delete triad1;
    delete triad2;
    delete triad3;
    delete triad4;
}

void MyQWhipple::setDoUpright(bool b) {
  doUpright = b;
}

bool MyQWhipple::getDoUpright() {
  return doUpright;
}

void MyQWhipple::initUprightTable() {

  // axis names
  int NeigPerBike = 4;
  std::vector<std::string> eignames(NeigPerBike);
  eignames[0] = "eig1";
  eignames[1] = "eig2";
  eignames[2] = "eig3";
  eignames[3] = "eig4";
  std::string linename;

  // x-axis
  eigPlotVTKTable = vtkSmartPointer<vtkTable>::New();
  eigX = vtkSmartPointer<vtkFloatArray>::New();
  eigX->SetName("forward speed (m/s)");
  eigPlotVTKTable->AddColumn(eigX);
  eigY.resize(1*NeigPerBike);

  // initialize y-axis
  for (int i = 0; i < NeigPerBike; i++) {
    linename = name + "-" + eignames[i]; 
    eigY[i] = vtkSmartPointer<vtkFloatArray>::New();
    eigY[i]->SetName(linename.c_str());
    eigPlotVTKTable->AddColumn(eigY[i]);
  }
}

void MyQWhipple::calcUpright(struct uprightOptions upOpts) {
  // next 3 lines taken from whippleeig.cpp's processOptions()
  bike->evalConstants();
  bike->eoms();
  bike->computeOutputs();

  // Vector to store range of speeds to calculate eigenvalues
  gsl_vector * speed = linspaceN(upOpts.vi, upOpts.vf, upOpts.N);

  // next 3 lines taken from whippleeig.cpp's main()
  bike->u1 = 0.0;
  bike->u3 = 0.0;
  bike->evalConstants();

  eigPlotVTKTable->SetNumberOfRows(upOpts.N);
  for (int i = 0; i < upOpts.N; ++i) {
    bike->u5 = -gsl_vector_get(speed, i)/(bike->rf+bike->rft);
    bike->calcEvals();
    eigPlotVTKTable->SetValue(i, 0, *gsl_vector_ptr(speed, i) );

    eigPlotVTKTable->SetValue(i, 1, bike->fourValues[0]);
    eigPlotVTKTable->SetValue(i, 2, bike->fourValues[1]);
    if (bike->fourValues[1] >= 0) {
      eigPlotVTKTable->SetValue(i, 3, fabs(bike->fourValues[2]));
    } else {
      eigPlotVTKTable->SetValue(i, 3, bike->fourValues[2]);
    } // if
    eigPlotVTKTable->SetValue(i, 4, bike->fourValues[3]);
  } // for i

  gsl_vector_free(speed);

  eigPlotVTKTable->Update();
}

vtkSmartPointer<vtkTable> MyQWhipple::GetUprightTable() {
  return eigPlotVTKTable;
}

void MyQWhipple::initSim(vtkSmartPointer<vtkRenderer> ren) {
  simRenderer = ren;
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
  rearConeRightTransform->Translate(0, hubhalfwidth/2, 0);
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
  rearConeLeftTransform->Translate(0,-hubhalfwidth/2, 0);
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
  rearCOMTransform->Translate(bike->lrx, 0, bike->lrz);
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
  rearTubeTransform->Translate(0,-bike->lr/2, 0);
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
  frontTubeTransform->Translate(0,-bike->ls/2, 0);
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
  frontOffsetTransform->Translate(bike->ls,-bike->lf/2, 0);
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
  frontConeRightSource->SetRadius(bike->rf);
  frontConeRightSource->SetResolution(100);
  frontConeRightSource->Update();
  // transform
  frontConeRightTransform = vtkTransform::New();
  frontConeRightTransform->Identity();
  frontConeRightTransform->Translate(0, hubhalfwidth/2, 0);
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
  frontConeLeftSource->SetRadius(bike->rf);
  frontConeLeftSource->SetResolution(100);
  frontConeLeftSource->Update();
  // transform
  frontConeLeftTransform = vtkTransform::New();
  frontConeLeftTransform->Identity();
  frontConeLeftTransform->Translate(0,-hubhalfwidth/2, 0);
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
  frontTorus->SetRingRadius(bike->rf);
  frontTorus->SetCrossSectionRadius(bike->rft);
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
  triad0 = new myvtkTriad(simRenderer);
  triad0->SetScale(.4,.4,.4);
  triad1 = new myvtkTriad(simRenderer);
  triad1->SetScale(.2,.2,.2);
  triad2 = new myvtkTriad(simRenderer);
  triad2->SetScale(.2,.2,.2);
  triad3 = new myvtkTriad(simRenderer);
  triad3->SetScale(.2,.2,.2);
  triad4 = new myvtkTriad(simRenderer);
  triad4->SetScale(.2,.2,.2);

  // ground reaction forces
  rearReaction = new myvtkTriad(simRenderer);
//  rearReaction->SetScale(2, 2, 2);
  rearReaction->SetColor(0, 0, 0);

  // wheel traces
  rearPoints = vtkSmartPointer<vtkPoints>::New();
  rearPoints->SetNumberOfPoints(1000);
//  rearPoints->InsertNextPoint(3, 0, 0);
  rearPointsLine = vtkSmartPointer<vtkPolyLine>::New();
  rearPointsLine->GetPointIds()->SetNumberOfIds(1000);
  for (unsigned int i = 0; i < 1000; i++) {
    rearPoints->SetPoint(i, i, 0, 0);
    rearPointsLine->GetPointIds()->SetId(i, i);
  }
//  rearPointsLine->GetPointIds()->InsertNextId(1);
  rearPointsCell = vtkSmartPointer<vtkCellArray>::New();
  rearPointsCell->InsertNextCell(rearPointsLine);
  rearPointsData = vtkSmartPointer<vtkPolyData>::New();
  rearPointsData->SetPoints(rearPoints);
  rearPointsData->SetLines(rearPointsCell);
  rearPointsMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  rearPointsMapper->SetInput(rearPointsData);
  rearPointsActor = vtkSmartPointer<vtkActor>::New();
  rearPointsActor->SetMapper(rearPointsMapper);
  rearPointsActor->GetProperty()->SetColor(1, 0, 0);
  // render
  simRenderer->AddActor(rearWheelAssy);
  simRenderer->AddActor(rearFrameAssy);
  simRenderer->AddActor(frontFrameAssy);
  simRenderer->AddActor(frontWheelAssy);
  simRenderer->AddActor(rearPointsActor);

  // sim data storage
  simVarNames.resize(NMOTIONVARS);
  simVarNames[0] = "t";
  simVarNames[1] = "q0";
  simVarNames[2] = "q1";
  simVarNames[3] = "q2";
  simVarNames[4] = "q3";
  simVarNames[5] = "q4";
  simVarNames[6] = "q5";
  simVarNames[7] = "q6";
  simVarNames[8] = "q7";
  simVarNames[9] = "u0";
  simVarNames[10] = "u1";
  simVarNames[11] = "u2";
  simVarNames[12] = "u3";
  simVarNames[13] = "u4";
  simVarNames[14] = "u5";
  simVarNames[15] = "fnx";
  simVarNames[16] = "fny";
  simVarNames[17] = "fnz";
  simVarNames[18] = "Rx";
  simVarNames[19] = "Ry";
  simVarNames[20] = "Rz";
  simVarNames[21] = "Fx";
  simVarNames[22] = "Fy";
  simVarNames[23] = "Fz";
  simVarNames[24] = "KE";
  simVarNames[25] = "PE";
  simVarNames[26] = "fa_yaw";
  simVarNames[27] = "fa_lean";
  simVarNames[28] = "fa_pitch";
  simVarNames[29] = "nh0";
  simVarNames[30] = "nh1";
  simVarNames[31] = "nh2";
  simTable = vtkSmartPointer<vtkTable>::New();
  // need animation runtime control. max time or add to the array somehow.
  // maybe set so that the filesize of output jpgs etc is < 1GB.
  simArrays.resize(NMOTIONVARS);

  for (uint i = 0; i < simArrays.size(); i++) {
    simArrays[i] = vtkSmartPointer<vtkFloatArray>::New();
    simArrays[i]->SetName(simVarNames[i].c_str());
    simTable->AddColumn(simArrays[i]);
  }
  simTable->InsertNextBlankRow();
  simTable->InsertNextBlankRow(1);
//  simTable->SetNumberOfRows(100);
}

Whipple* MyQWhipple::getBike() {
  return bike;
}

std::string MyQWhipple::getName() {
  return name;
}

void MyQWhipple::setGyroParams( WhippleParams *p) {
  *gswp = *p;
}

void MyQWhipple::setMeijParams( MJWhippleParams *p) {
  *mjwp = *p;
}

WhippleParams* MyQWhipple::getGyroParams() {
  return gswp;
}

MJWhippleParams* MyQWhipple::getMeijParams() {
  return mjwp;
}

void MyQWhipple::setParamType( int p) {
  paramtype = p;
}

int MyQWhipple::getParamType() {
  return paramtype;
}

void MyQWhipple::SimUpdate() {
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
  rearWheelAssy->SetOrientation(0, 0, 0);
  rearWheelAssy->RotateZ(180/M_PI*YAW);// NEGATIVE? IS RIGHTWARD AS IS

  rearReaction->SetPosition(X, Y, 0);
  rearReaction->SetOrientation(rearWheelAssy->GetOrientation());
  rearReaction->SetRelScale(0, bike->Rx/bike->mr/bike->g);
  rearReaction->SetRelScale(1, bike->Ry/bike->mr/bike->g);
  rearReaction->SetRelScale(2, bike->Rz/bike->mr/bike->g);
  
  rearWheelAssy->RotateX(180/M_PI*LEAN);
  rearWheelAssy->RotateY(180/M_PI*RWSPIN);
  
  triad0->SetPosition(X, Y, 0);
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
  frontFrameAssy->SetPosition(0, 0,-1);

  // front frame transform
  frontFrameTransform->SetMatrix(rearFrameAssy->GetMatrix());
  frontFrameTransform->Translate(bike->lr, 0, 0);
  frontFrameTransform->RotateY(-90);
  frontFrameTransform->RotateX(-STEER);
  frontFrameAssy->SetPosition(frontFrameTransform->GetPosition());
  frontFrameAssy->SetOrientation(frontFrameTransform->GetOrientation());

  triad3->SetPosition(frontFrameAssy->GetPosition());
  triad3->SetOrientation(frontFrameAssy->GetOrientation());

  // front wheel assembly
  frontWheelAssy->SetPosition(0, 0,-bike->rf - bike->rft);
  
  // front frame transform
  frontWheelTransform->SetMatrix(frontFrameAssy->GetMatrix());
  frontWheelTransform->Translate(bike->ls, 0,-bike->lf);
  frontWheelTransform->RotateY(90-180/M_PI*(PITCH-FWSPIN));
  frontWheelAssy->SetPosition(frontWheelTransform->GetPosition());
  frontWheelAssy->SetOrientation(frontWheelTransform->GetOrientation());

  triad4->SetPosition(frontWheelAssy->GetPosition());
  triad4->SetOrientation(frontWheelAssy->GetOrientation());
//      iren->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->ResetCamera();
//    simRenderer->ResetCameraClippingRange();
//
  // wheel traces
//  for  get timer count, maybe
 // rearPoints->InsertNextPoint(X, Y, 0);
 rearPoints->SetPoint(simTable->GetNumberOfRows(), X, Y, 0);
 rearPointsMapper->Update();
//  rearPointsLine->GetPointIds()->SetNumberOfIds(simTable->GetNumberOfRows());
 // for (unsigned int i = 0; i < simTable->GetNumberOfRows(); i++) {
    //rearPointsLine->GetPointIds()->SetId(i, i);
//rearPointsLine->GetPointIds()->InsertNextId(simTable->GetNumberOfRows());
//  }
  // camera
  double d = 1;
  double pos[3];
  pos[0] = frontFrameAssy->GetPosition()[0];
  pos[1] = frontFrameAssy->GetPosition()[1];
  pos[2] = frontFrameAssy->GetPosition()[2];
//  double bounds[6] = frontFrameAssy->GetBounds();
  simRenderer->ResetCamera(pos[0]-d, pos[0]+d,
      pos[1]-d, pos[1]+d,
      pos[2]-d, pos[2]+d);
//  simRenderer->GetActiveCamera()->Dolly(.5);

 // simRenderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
  //frontFrameAssy->AddPosition(0, 5,-2);
  //simRenderer->GetActiveCamera()->SetPosition(frontFrameAssy->GetPosition());
//  simRenderer->GetActiveCamera()->SetDistance(5);
//  simRenderer->GetActiveCamera()->SetViewUp(0, 0,-1);
//  simRenderer->GetActiveCamera()->Elevation(-95);
//  simRenderer->GetActiveCamera()->SetFocalPoint(frontFrameAssy->GetPosition());
}

void MyQWhipple::SetSimValues(int rowidx) {
   // do some nice memory managemen here to control the size of the array
  simTable->InsertNextBlankRow();
 /* if (rowidx > simTable->GetNumberOfRows()) {
    simTable->SetNumberOfRows(10000+simTable->GetNumberOfRows());
  } */
  simTable->SetValue(rowidx, 0, bike->t);
  simTable->SetValue(rowidx, 1, bike->q0);
  simTable->SetValue(rowidx, 2, bike->q1);
  simTable->SetValue(rowidx, 3, bike->q2);
  simTable->SetValue(rowidx, 4, bike->q3);
  simTable->SetValue(rowidx, 5, bike->q4);
  simTable->SetValue(rowidx, 6, bike->q5);
  simTable->SetValue(rowidx, 7, bike->q6);
  simTable->SetValue(rowidx, 8, bike->q7);
  simTable->SetValue(rowidx, 9, bike->u0);
  simTable->SetValue(rowidx, 10, bike->u1);
  simTable->SetValue(rowidx, 11, bike->u2);
  simTable->SetValue(rowidx, 12, bike->u3);
  simTable->SetValue(rowidx, 13, bike->u4);
  simTable->SetValue(rowidx, 14, bike->u5);
  simTable->SetValue(rowidx, 15, bike->no_fn[0]);
  simTable->SetValue(rowidx, 16, bike->no_fn[1]);
  simTable->SetValue(rowidx, 17, bike->no_fn[2]);
  simTable->SetValue(rowidx, 18, bike->Rx);
  simTable->SetValue(rowidx, 19, bike->Ry);
  simTable->SetValue(rowidx, 20, bike->Rz);
  simTable->SetValue(rowidx, 21, bike->Fx);
  simTable->SetValue(rowidx, 22, bike->Fy);
  simTable->SetValue(rowidx, 23, bike->Fz);
  simTable->SetValue(rowidx, 24, bike->ke);
  simTable->SetValue(rowidx, 25, bike->pe);
  simTable->SetValue(rowidx, 26, bike->fa_yaw);
  simTable->SetValue(rowidx, 27, bike->fa_lean);
  simTable->SetValue(rowidx, 28, bike->fa_pitch);
  simTable->SetValue(rowidx, 29, bike->constraints[0]);
  simTable->SetValue(rowidx, 30, bike->constraints[1]);
  simTable->SetValue(rowidx, 31, bike->constraints[2]);
  simTable->Update();
}

vtkSmartPointer<vtkTable> MyQWhipple::GetSimTable() {
  return simTable;
}

void MyQWhipple::writeSim(std::string fname) {
  simTable->Update();
  std::ofstream fid(fname.c_str());
  for (int i = 0; i < simTable->GetNumberOfRows(); i++) {
    for (int j = 0; j < NMOTIONVARS; j++) {
      fid << simTable->GetValue(i, j) << " ";
    }
    fid << endl;
  }
  fid.close();
}

// use VTKCONTEXT2D TO DRAW 2D BIKE
void MyQWhipple::Draw2D(vtkSmartPointer<vtkContext2D> context) {
  // must solve for q2
  context->GetPen()->SetColorF(1, 1, 1);
  context->GetPen()->SetWidth(5);
  double rearrad = bike->rr+bike->rrt;
context->DrawEllipse(0, rearrad, bike->rr, bike->rr);
context->DrawEllipse(0, rearrad, rearrad, rearrad);
double x1 = bike->lr*cos(bike->q2);
double y1 = rearrad + bike->lr*sin(bike->q2);
double x2 = x1 + bike->ls*cos(90-bike->q2);
double y2 = y1 - bike->ls*sin(90-bike->q2);
double x3 = x2 + bike->lf*cos(bike->q2);
double y3 = y2 + bike->lf*sin(bike->q2);
context->DrawLine(0, rearrad, x1, y1);
context->DrawLine(x1, y1, x2, y2);
context->DrawLine(x2, y2, x3, y3);
double frontrad = bike->rf+bike->rft;
context->DrawEllipse(x3, y3, bike->rf, bike->rf);
context->DrawEllipse(x3, y3, frontrad, frontrad);
//draw mass centres
}

void MyQWhipple::QDraw2D(QGraphicsScene* qscene) {

  qscene->clear();

  bike->calcPitch();
  QPen qframePen(Qt::red);
  qframePen.setWidthF(.09);
  QPen qwheelPen(Qt::black);
  qwheelPen.setWidthF(.05);

  double rearrad = bike->rr+bike->rrt;
  qscene->addEllipse(-rearrad+bike->rrt, 0, 2*bike->rr,-2*bike->rr, qwheelPen);
  qscene->addEllipse(-rearrad-bike->rrt, 0, 2*rearrad,-2*rearrad, qwheelPen);
  double x1 = bike->lr*cos(bike->q2);
  double y1 = rearrad + bike->lr*sin(bike->q2);
  double x2 = x1 + bike->ls*cos(M_PI/2-bike->q2);
  double y2 = y1 - bike->ls*sin(M_PI/2-bike->q2);
  double x3 = x2 + bike->lf*cos(bike->q2);
  double y3 = y2 + bike->lf*sin(bike->q2);
  qscene->addLine(0,-rearrad, x1,-y1, qframePen);
  qscene->addLine(x1,-y1, x2,-y2, qframePen);
  qscene->addLine(x2,-y2, x3,-y3, qframePen);
  double frontrad = bike->rf+bike->rft;
  qscene->addEllipse(x3-frontrad,-y3+frontrad, 2*bike->rf,-2*bike->rf, qwheelPen);
  qscene->addEllipse(x3-frontrad,-y3+frontrad, 2*frontrad,-2*frontrad, qwheelPen);
  qscene->addLine(-rearrad, 0, x3+frontrad, 0);
}

void MyQWhipple::UpdateTrace() {
    for (int i = 0; i < simTable->GetNumberOfRows(); i++) {
      rearPoints->SetPoint(i, simTable->GetValue(i, 7).ToDouble(), simTable->GetValue(i, 8).ToDouble(), 0.0);
    }
    rearPointsMapper->Update();
  }

void MyQWhipple::TurnOffReactionTriads() {
  rearReaction->SetVisibility(0);
}

void MyQWhipple::TurnOnReactionTriads() {
  rearReaction->SetVisibility(1);
}

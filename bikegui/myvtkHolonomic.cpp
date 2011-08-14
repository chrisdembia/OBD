// c
#include <cmath>

// vtk
#include <vtkImplicitFunction.h>
#include <vtkObjectFactory.h>

// gui
#include "myvtkHolonomic.h"

vtkStandardNewMacro(myvtkHolonomic);

myvtkHolonomic::myvtkHolonomic() {};

void myvtkHolonomic::SetBike(Whipple *bike)
{
  rft = bike->rft;
  rr = bike->rr;
  rrt = bike->rrt;
  rf = bike->rf;
  lr = bike->lr;
  ls = bike->ls;
  lf = bike->lf;
} // SetBike()

double myvtkHolonomic::EvaluateFunction(double x[3])
{
  double term1 = (sin(x[0])*cos(x[2])+sin(x[1])*sin(x[2])*cos(x[0]));
  return rft + ls*cos(x[0])*cos(x[1])+
    lf*(sin(x[0])*sin(x[2])-sin(x[1])*cos(x[0])*cos(x[2])) +
    rf*sqrt(1-term1*term1)
    -rrt -lr*sin(x[1])*cos(x[0]) - rr*fabs(cos(x[0]));
} // EvaluateFunction()

void myvtkHolonomic::EvaluateGradient(double x[3], double n[3])
{
  // not quite implemented yet =p
  n[0] = 1;
  n[1] = 1;
  n[2] = 1;
} // EvaluateGradient()

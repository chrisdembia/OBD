

#include <vtkImplicitFunction.h>
#include <vtkObjectFactory.h>
#include <cmath>

#include "myvtkHolonomic.h"

vtkStandardNewMacro(myvtkHolonomic);

myvtkHolonomic::myvtkHolonomic()
{
rft = 0.0;
rr = 0.3;
rrt = 0.0;
rf = 0.35;
lr = 0.9535;
ls = 0.268;
lf = 0.0321;
Radius = 0.5;
Center[0] = 0.0;
Center[1] = 0.0;
Center[2] = 0.0;
}

double myvtkHolonomic::EvaluateFunction(double x[3])
{
    //return rft + ls*cos(x[0])*cos(x[1])+ lf*(sin(x[0])*sin(x[2])-sin(x[1])*cos(x[0])*cos(x[2])) + rf*sqrt(1-pow((sin(x[0])*cos(x[2])+sin(x[1])*sin(x[2])*sin(x[0])),2)) -rrt -lr*sin(x[1])*cos(x[0]) - rr*pow(cos(x[0]),2)/abs(cos(x[0]));
//return x[1]*x[1];
return 1/pow(x[0],10) - x[1];
//return ( ((x[0]-Center[0]) * (x[0]-Center[0]) + (x[1]-Center[1])*(x[1]-Center[1]) + (x[2]-Center[2])*(x[2]-Center[2])) - Radius*Radius);
}

void myvtkHolonomic::EvaluateGradient(double x[3], double n[3])
{
    n[0] = 1;
    n[1] = 1;
    n[2] = 1;
//    n[0] = 2.0*( x[0] - Center[0]);
//    n[1] = 2.0*(x[1] - Center[1]);
 //   n[2] = 2.0*(x[2] - Center[2]);
}

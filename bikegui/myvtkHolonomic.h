#ifndef __myvtkHolonomic_h
#define __myvtkHolonomic_h

// c
#include <cmath>

// vtk
class vtkImplicitFunction;

// OBD
#include "whipple.h"

class VTK_FILTERING_EXPORT myvtkHolonomic : public vtkImplicitFunction
{
    public:
        vtkTypeMacro(myvtkHolonomic,vtkImplicitFunction);
        static myvtkHolonomic *New();

        double EvaluateFunction(double x[3]);
        void EvaluateGradient(double x[3], double n[3]);
        void SetBike(Whipple*bike);
    protected:
        myvtkHolonomic();
        ~myvtkHolonomic() {};
        double rft;
        double rr;
        double rrt;
        double rf;
        double lr;
        double ls;
        double lf;
    private:
        myvtkHolonomic(const myvtkHolonomic&);
        void operator=(const myvtkHolonomic&);

};

#endif

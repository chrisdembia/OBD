#ifndef __myvtkHolonomic_h
#define __myvtkHolonomic_h

#include <vtkImplicitFunction.h>
#include <cmath>


class VTK_FILTERING_EXPORT myvtkHolonomic : public vtkImplicitFunction
{
    public:
        vtkTypeMacro(myvtkHolonomic,vtkImplicitFunction);
        static myvtkHolonomic* New();

        double EvaluateFunction(double x[3]);
        double EvaluateFunction(double x, double y, double z)
        { return this->myvtkHolonomic::EvaluateFunction(x,y,z);};
        void EvaluateGradient(double x[3], double n[3]);
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
        double Radius;
        double Center[3];
    private:
        myvtkHolonomic(const myvtkHolonomic&);
        void operator=(const myvtkHolonomic&);

};

#endif

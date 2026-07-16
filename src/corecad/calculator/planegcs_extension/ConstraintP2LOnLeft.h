#pragma once

#include <GCS.h>

namespace GCS
{
    class ConstraintP2LOnLeft : public Constraint
    {
    private:
        double* p0x() { return pvec[0]; }
        double* p0y() { return pvec[1]; }
        double* p1x() { return pvec[2]; }
        double* p1y() { return pvec[3]; }
        double* p2x() { return pvec[4]; }
        double* p2y() { return pvec[5]; }
        double* distance() { return pvec[6]; }

    public:
        // 3-Point Constructor Overload (Bypasses Line object instantiation)
        ConstraintP2LOnLeft(Point& p0, Point& p1, Point& p2, double* d)
        {
            pvec.push_back(p0.x);
            pvec.push_back(p0.y);
            pvec.push_back(p1.x);
            pvec.push_back(p1.y);
            pvec.push_back(p2.x);
            pvec.push_back(p2.y);
            pvec.push_back(d);
            origpvec = pvec;
            rescale();
        }

    #ifdef _GCS_EXTRACT_SOLVER_SUBSYSTEM_
        ConstraintP2LOnLeft() {}
    #endif

        ConstraintType getTypeId() override { return static_cast<ConstraintType>(ArcLength + 1); }
        double error() override;
        double grad(double*) override;
        double maxStep(MAP_pD_D& dir, double lim = 1.) override;
    };
}

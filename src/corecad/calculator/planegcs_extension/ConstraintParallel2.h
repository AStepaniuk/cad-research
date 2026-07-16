#pragma once

#include <GCS.h>

namespace GCS
{
    class ConstraintParallel2: public Constraint
    {
    private:
        double* l1p1x()
        {
            return pvec[0];
        }
        double* l1p1y()
        {
            return pvec[1];
        }
        double* l1p2x()
        {
            return pvec[2];
        }
        double* l1p2y()
        {
            return pvec[3];
        }
        double* l2p1x()
        {
            return pvec[4];
        }
        double* l2p1y()
        {
            return pvec[5];
        }
        double* l2p2x()
        {
            return pvec[6];
        }
        double* l2p2y()
        {
            return pvec[7];
        }

    public:
        ConstraintParallel2(Point& l1s, Point& l1e, Point& l2s, Point& l2e);
    #ifdef _GCS_EXTRACT_SOLVER_SUBSYSTEM_
        ConstraintParallel2()
        {}
    #endif
        ConstraintType getTypeId() override;
        void rescale(double coef = 1.) override;
        double error() override;
        double grad(double*) override;
    };
}

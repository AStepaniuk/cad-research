#include "ConstraintParallel2.h"

using namespace GCS;

ConstraintParallel2::ConstraintParallel2(Point& l1s, Point& l1e, Point& l2s, Point& l2e)
{
    pvec.push_back(l1s.x);
    pvec.push_back(l1s.y);
    pvec.push_back(l1e.x);
    pvec.push_back(l1e.y);
    pvec.push_back(l2s.x);
    pvec.push_back(l2s.y);
    pvec.push_back(l2e.x);
    pvec.push_back(l2e.y);
    origpvec = pvec;
    ConstraintParallel2::rescale();
}

ConstraintType ConstraintParallel2::getTypeId()
{
    return Parallel;
}

void ConstraintParallel2::rescale(double coef)
{
    double dx1 = (*l1p1x() - *l1p2x());
    double dy1 = (*l1p1y() - *l1p2y());
    double dx2 = (*l2p1x() - *l2p2x());
    double dy2 = (*l2p1y() - *l2p2y());
    scale = coef / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2));
}

double ConstraintParallel2::error()
{
    double dx1 = (*l1p1x() - *l1p2x());
    double dy1 = (*l1p1y() - *l1p2y());
    double dx2 = (*l2p1x() - *l2p2x());
    double dy2 = (*l2p1y() - *l2p2y());
    return scale * (dx1 * dy2 - dy1 * dx2);
}

double ConstraintParallel2::grad(double* param)
{
    double deriv = 0.;
    if (param == l1p1x()) {
        deriv += (*l2p1y() - *l2p2y());  // = dy2
    }
    if (param == l1p2x()) {
        deriv += -(*l2p1y() - *l2p2y());  // = -dy2
    }
    if (param == l1p1y()) {
        deriv += -(*l2p1x() - *l2p2x());  // = -dx2
    }
    if (param == l1p2y()) {
        deriv += (*l2p1x() - *l2p2x());  // = dx2
    }

    if (param == l2p1x()) {
        deriv += -(*l1p1y() - *l1p2y());  // = -dy1
    }
    if (param == l2p2x()) {
        deriv += (*l1p1y() - *l1p2y());  // = dy1
    }
    if (param == l2p1y()) {
        deriv += (*l1p1x() - *l1p2x());  // = dx1
    }
    if (param == l2p2y()) {
        deriv += -(*l1p1x() - *l1p2x());  // = -dx1
    }

    return scale * deriv;
}

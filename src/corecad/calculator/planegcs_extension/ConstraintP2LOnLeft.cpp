#include "ConstraintP2LOnLeft.h"

using namespace GCS;

double ConstraintP2LOnLeft::error()
{
    double x0 = *p0x(), x1 = *p1x(), x2 = *p2x();
    double y0 = *p0y(), y1 = *p1y(), y2 = *p2y();
    double dist = *distance();
    
    double dx = x2 - x1;
    double dy = y2 - y1;
    double d = sqrt(dx * dx + dy * dy); // Line length
    if (d < 1e-9) return 0.0;
    
    // Signed area/cross product layout. 
    // Stripping std::abs forces a single algebraic root on the desired side.
    double signed_area = -x0 * dy + y0 * dx + x1 * y2 - x2 * y1;
    
    return scale * (signed_area / d - dist);
}

double ConstraintP2LOnLeft::grad(double* param)
{
    double deriv = 0.;

    if (param == p0x() || param == p0y() || param == p1x() || param == p1y() || param == p2x() || param == p2y()) {
        double x0 = *p0x(), x1 = *p1x(), x2 = *p2x();
        double y0 = *p0y(), y1 = *p1y(), y2 = *p2y();
        
        double dx = x2 - x1;
        double dy = y2 - y1;
        double d2 = dx * dx + dy * dy;
        double d = sqrt(d2);
        if (d < 1e-9) return 0.0;
        
        double signed_area = -x0 * dy + y0 * dx + x1 * y2 - x2 * y1;
        
        if (param == p0x()) {
            deriv += (y1 - y2) / d;
        }
        if (param == p0y()) {
            deriv += (x2 - x1) / d;
        }
        if (param == p1x()) {
            deriv += ((y2 - y0) * d + (dx / d) * signed_area) / d2;
        }
        if (param == p1y()) {
            deriv += ((x0 - x2) * d + (dy / d) * signed_area) / d2;
        }
        if (param == p2x()) {
            deriv += ((y0 - y1) * d - (dx / d) * signed_area) / d2;
        }
        if (param == p2y()) {
            deriv += ((x1 - x0) * d - (dy / d) * signed_area) / d2;
        }
        // REMOVED: if (area < 0) { deriv *= -1; } 
        // Analytical gradients match the signed_area definition exactly.
    }
    
    if (param == distance()) {
        deriv += -1;
    }

    return scale * deriv;
}

double ConstraintP2LOnLeft::maxStep(MAP_pD_D& dir, double lim)
{
    MAP_pD_D::iterator it;
    it = dir.find(distance());
    if (it != dir.end()) {
        if (it->second < 0.) {
            lim = std::min(lim, -(*distance()) / it->second);
        }
    }
    
    // Restrict parameter tracking step to ensure the step direction 
    // doesn't cause chaotic solver updates across severe coordinate jumps.
    double darea = 0.;
    double x0 = *p0x(), x1 = *p1x(), x2 = *p2x();
    double y0 = *p0y(), y1 = *p1y(), y2 = *p2y();
    
    if ((it = dir.find(p0x())) != dir.end()) darea += (y1 - y2) * it->second;
    if ((it = dir.find(p0y())) != dir.end()) darea += (x2 - x1) * it->second;
    if ((it = dir.find(p1x())) != dir.end()) darea += (y2 - y0) * it->second;
    if ((it = dir.find(p1y())) != dir.end()) darea += (x0 - x2) * it->second;
    if ((it = dir.find(p2x())) != dir.end()) darea += (y0 - y1) * it->second;
    if ((it = dir.find(p2y())) != dir.end()) darea += (x1 - x0) * it->second;

    // Use absolute value of change because we limit the absolute speed of motion step bounds
    double abs_darea = std::abs(darea);
    if (abs_darea > 0.) {
        double dx = x2 - x1;
        double dy = y2 - y1;
        double area_limit = 0.3 * (*distance()) * sqrt(dx * dx + dy * dy);
        
        if (abs_darea > area_limit) {
            // Evaluated against the signed cross-product magnitude
            area_limit = std::max(area_limit, 0.3 * std::abs(-x0 * dy + y0 * dx + x1 * y2 - x2 * y1));
            if (abs_darea > area_limit) {
                lim = std::min(lim, area_limit / abs_darea);
            }
        }
    }
    return lim;
}

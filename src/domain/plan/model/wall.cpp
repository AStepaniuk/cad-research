#include "wall.h"

std::ostream& domain::plan::model::operator<<(std::ostream &os, const wall &w)
{
    return os << static_cast<const corecad::model::model_base<wall>&>(w)
        << " s:" << w.start << " e:" << w.end << " w:" << w.width
        << " sl:" << w.start_left << " sr:" << w.start_right
        << " el:" << w.end_left << " er:" << w.end_right;
}

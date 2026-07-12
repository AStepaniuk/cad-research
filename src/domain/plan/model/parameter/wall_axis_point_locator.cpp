#include "wall_axis_point_locator.h"

std::ostream& domain::plan::model::parameter::operator<<(std::ostream &os, const wall_axis_point_locator &l)
{
    return os << l.wid << '-' << l.point_on_axis_ptr;
}

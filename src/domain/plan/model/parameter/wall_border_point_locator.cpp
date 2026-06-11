#include "wall_border_point_locator.h"

std::ostream& domain::plan::model::parameter::operator<<(std::ostream &os, const wall_border_point_locator &l)
{
    return os << l.wid << '-' << l.border_ptr << l.point_on_border_ptr;
}

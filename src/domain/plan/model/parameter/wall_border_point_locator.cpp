#include "wall_border_point_locator.h"


using namespace domain::plan::model::parameter;

std::ostream& operator<<(std::ostream &os, const wall_border_point_locator &l)
{
    return os << l.wid << '-' << l.border_ptr << l.point_on_border_ptr;
}

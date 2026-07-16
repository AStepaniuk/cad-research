#pragma once

#include "wall.h"

namespace domain::plan::model::parameter
{
    struct wall_axis_point_locator
    {
        shape::wall::index_t wid;
        shape::point_on_wall_axis_ptr point_on_axis_ptr;

        bool operator<=>(const wall_axis_point_locator&) const = default;
    };
    
    std::ostream& operator<<(std::ostream& os, const wall_axis_point_locator& l);
}

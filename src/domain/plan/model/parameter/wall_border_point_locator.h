#pragma once

#include "wall.h"

namespace domain::plan::model::dimension
{
    struct wall_border_point_locator
    {
        wall::index_t wid;
        wall_border_line_ptr border_ptr;
        point_on_wall_border_ptr point_on_border_ptr;
    };
    
    std::ostream& operator<<(std::ostream& os, const wall_border_point_locator& l);
}

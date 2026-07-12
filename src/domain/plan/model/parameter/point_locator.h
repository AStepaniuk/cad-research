#pragma once

#include <variant>

#include "wall_axis_point_locator.h"
#include "wall_border_point_locator.h"

namespace domain::plan::model::parameter
{
    using point_locator_t = std::variant<wall_axis_point_locator, wall_border_point_locator>;

    std::ostream& operator<<(std::ostream& os, const point_locator_t& l);
}

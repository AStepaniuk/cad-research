#include "floor_query.h"

using namespace domain::plan::calculator;
using namespace domain::plan::model::parameter;
using namespace domain::plan::model::shape;

floor_query::floor_query(model::floor &floor)
    : _floor { floor }
{
}

std::vector<point_locator_t> floor_query::find_locators(wall_axis_point::index_t pid)
{
    std::vector<point_locator_t> result;

    for (const auto& wp : _floor.data().items<wall>())
    {
        const auto& al = _floor.data().get(wp.second.axis);

        if (al.s == pid)
        {
            result.push_back(wall_axis_point_locator { wp.first, &wall_axis_line::s });
        }

        if (al.e == pid)
        {
            result.push_back(wall_axis_point_locator { wp.first, &wall_axis_line::e });
        }
    }

    return result;
}

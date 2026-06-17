#include "point_resolver.h"

#include "overloaded.h"

using namespace domain::plan::resolver;
using namespace domain::plan::model::parameter;
using namespace domain::plan::model::shape;

point_resolver::point_resolver(const model::floor &floor)
    : _floor { floor }
{
}

point_resolver::point_index_t point_resolver::resolve(const point_locator_t &pl)
{
    return std::visit(corecad::util::overloaded
        {
            [&](const wall_border_point_locator& wbpl)
            {
                const auto& w = _floor.data().get(wbpl.wid);
                const auto& l = _floor.data().get(w.*(wbpl.border_ptr));

                return l.*(wbpl.point_on_border_ptr);
            }
        },
        pl
    );
}

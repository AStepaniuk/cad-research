#include "wall.h"

#include "overloaded.h"

std::ostream& domain::plan::model::shape::operator<<(std::ostream &os, const wall &w)
{
    return os << static_cast<const corecad::model::model_base<wall>&>(w)
        << " a:" << w.axis << " w:" << w.width
        << " l:" << w.left << " r:" << w.right
        << " ss:" << w.start_stub << " es:" << w.end_stub;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_axis_point_locator &wapl)
{
    return os << "wid:" << wapl.wall_id << "-" << wapl.point_on_axis_ptr;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_axis_point_data &wapd)
{
    if (wapd.connected_walls)
    {
        bool first = true;
        for (const auto& wapl : wapd.connected_walls.value())
        {
            if (!first) os << " | ";
            os << wapl;

            first = false;
        }

        return os;
    }
    else
    {
        return os << "[empty]";
    }
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_points_tl::variant_t &val)
{
    std::visit(corecad::util::overloaded {
            [&](const wall_axis_point& v) { os << 'a' << v; },
            [&](const wall_border_point& v) { os << 'b' << v; },
        }
        , val
    );
    return os;
}

std::ostream& domain::plan::model::shape::operator<<(std::ostream& os, const wall_point_id_t& val)
{
    std::visit(corecad::util::overloaded {
            [&](const wall_axis_point::index_t& v) { os << 'a' << v; },
            [&](const wall_border_point::index_t& v) { os << 'b' << v; },
        }
        , val
    );
    return os;
}

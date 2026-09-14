#include "wall.h"

#include "overloaded.h"
#include "std_ostream_extensions.h"

std::ostream& domain::plan::model::shape::operator<<(std::ostream &os, const wall &w)
{
    return os << static_cast<const corecad::model::model_base<wall>&>(w)
        << " a:" << w.axis << " t:" << w.thickness
        << " l:" << w.left << " r:" << w.right
        << " ss:" << w.start_stub << " es:" << w.end_stub;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_axis_point_locator &wapl)
{
    return os << "wid:" << wapl.wall_id << "-" << wapl.point_on_axis_ptr;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_axis_point_data &wapd)
{
    return os << wapd.connected_walls;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_border_line_ptr &wblp)
{
    if (wblp == &wall::left) return os << 'l';
    if (wblp == &wall::right) return os << 'r';
    if (wblp == &wall::start_stub) return os << "ss";
    if (wblp == &wall::end_stub) return os << "es";
    return os << "??";
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_border_point_locator &wbpl)
{
    return os << "wid:" << wbpl.wall_id << "-" << wbpl.border_ptr << wbpl.point_on_border_ptr;
}

std::ostream &domain::plan::model::shape::operator<<(std::ostream &os, const wall_border_point_data &wbpd)
{
    return os << wbpd.point_locators[0] << " | " << wbpd.point_locators[1];
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

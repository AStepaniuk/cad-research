#pragma once

#include "property.h"
#include "point_locator.h"
#include "coordinate2d.h"
#include "wall.h"

namespace domain::plan::model::parameter
{
    template<typename TModel>
    struct colinear
    {
        using point_id_t = shape::wall_point_id_t;

        colinear(point_id_t p1, point_id_t p2, point_id_t p3)
            : point1 { nullptr, p1 }
            , point2 { nullptr, p2 }
            , point3 { nullptr, p3 }
        {}

        corecad::model::property<point_id_t, TModel> point1;
        corecad::model::property<point_id_t, TModel> point2;
        corecad::model::property<point_id_t, TModel> point3;

        static constexpr auto members_metadata = std::make_tuple(
            &colinear::point1,
            &colinear::point2,
            &colinear::point3
        );
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const colinear<TModel>& c)
    {
        return os << " p1:" << c.point1 << " p2:" << c.point2 << " p3:" << c.point3;
    }
}

#pragma once

#include "property.h"
#include "coordinate2d.h"
#include "wall.h"
#include "member_info.h"

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

        struct metadata
        {
            static constexpr std::string_view type_name = "colinear"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("point1", &colinear::point1),
                corecad::meta::member("point2", &colinear::point2),
                corecad::meta::member("point3", &colinear::point3)
            );
        };
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const colinear<TModel>& c)
    {
        return os << " p1:" << c.point1 << " p2:" << c.point2 << " p3:" << c.point3;
    }
}

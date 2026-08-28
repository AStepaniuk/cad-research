#pragma once

#include "property.h"
#include "coordinate2d.h"
#include "wall.h"

namespace domain::plan::model::parameter
{
    template<typename TModel>
    struct pinned
    {
        using point_id_t = typename shape::wall_point_id_t;

        pinned(point_id_t p, double v, corecad::model::coordinate2d c)
            : coordinate { nullptr, c }
            , point { nullptr, p }
            , value { nullptr, v }
        {}

        corecad::model::property<corecad::model::coordinate2d, TModel> coordinate;
        corecad::model::property<point_id_t, TModel> point;
        corecad::model::property<double, TModel> value;

        struct metadata
        {
            static constexpr std::string_view type_name = "pinned"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("coordinate", &pinned::coordinate),
                corecad::meta::member("point", &pinned::point),
                corecad::meta::member("value", &pinned::value)
            );
        };
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const pinned<TModel>& p)
    {
        return os << "p:" << p.point << ": " << p.coordinate << "=" << p.value;
    }
}

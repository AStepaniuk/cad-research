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

        static constexpr auto members_metadata = std::make_tuple(
            &pinned::coordinate,
            &pinned::point,
            &pinned::value
        );
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const pinned<TModel>& p)
    {
        return os << "p:" << p.point << ": " << p.coordinate << "=" << p.value;
    }
}

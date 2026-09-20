#pragma once

#include "point2d.h"
#include "coordinate2d.h"
#include "property.h"
#include "type_list.h"

namespace corecad::model::constraint
{
    template <typename TPoint2DIndexList, typename TModel>
    requires util::AllElementsAre<TPoint2DIndexList, is_point2d_index>
    struct offset
    {
        using point_id_t = TPoint2DIndexList::variant_t;

        offset(point_id_t f, point_id_t t, double o, coordinate2d d)
            : direction { nullptr, d }
            , from { nullptr, f }
            , to { nullptr, t }
            , distance { nullptr, o }
        {}

        property<coordinate2d, TModel> direction;

        property<point_id_t, TModel> from;
        property<point_id_t, TModel> to;

        property<double, TModel> distance;

        struct metadata
        {
            static constexpr std::string_view type_name = "offset"; 

            static constexpr auto members = std::make_tuple(
                meta::member("direction", &offset::direction),
                meta::member("from", &offset::from),
                meta::member("to", &offset::to),
                meta::member("distance", &offset::distance)
            );
        };
    };

    template <typename TPoint2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const offset<TPoint2DIndexList, TModel>& o)
    {
        return os << (o.direction == coordinate2d::x ? "x" : "y")
            << " from:" << o.from << " to:" << o.to << " dist:" << o.distance;
    }
}

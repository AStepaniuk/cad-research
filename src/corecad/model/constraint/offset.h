#pragma once

#include "vector2d.h"
#include "coordinate2d.h"
#include "property.h"
#include "type_list.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct offset
    {
        using point_id_t = TVector2DIndexList::variant_t;

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

        static constexpr auto members_metadata = std::make_tuple(
            &offset::direction,
            &offset::from,
            &offset::to,
            &offset::distance
        );
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const offset<TVector2DIndexList, TModel>& o)
    {
        return os << (o.direction == coordinate2d::x ? "x" : "y")
            << " from:" << o.from << " to:" << o.to << " dist:" << o.distance;
    }
}

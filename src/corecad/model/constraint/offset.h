#pragma once

#include "vector2d.h"
#include "property.h"
#include "type_list.h"

namespace corecad::model::constraint
{
    enum class offset_direction { horizontal, vertical };
    
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct offset
    {
        using point_id_t = TVector2DIndexList::variant_t;

        offset(point_id_t f, point_id_t t, double o, offset_direction d)
            : direction { nullptr, d }
            , from { nullptr, f }
            , to { nullptr, t }
            , distance { nullptr, o }
        {}

        property<offset_direction, TModel> direction;

        property<point_id_t, TModel> from;
        property<point_id_t, TModel> to;

        property<double, TModel> distance;

        void bind(TModel& parent)
        {
            direction.bind(parent);
            from.bind(parent);
            to.bind(parent);
            distance.bind(parent);
        }

        void reset_properties_updated()
        {
            direction.reset_updated();
            from.reset_updated();
            to.reset_updated();
            distance.reset_updated();
        }
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const offset<TVector2DIndexList, TModel>& o)
    {
        return os << (o.direction == offset_direction::vertical ? "vertical" : "horizontal")
            << " from:" << o.from << " to:" << o.to << " dist:" << o.distance;
    }
}

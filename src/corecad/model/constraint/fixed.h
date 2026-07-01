#pragma once

#include "vector2d.h"
#include "property.h"

namespace corecad::model::constraint
{
    enum class fixed_coordinate { x, y };
    
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct fixed
    {
        using point_id_t = TVector2DIndexList::variant_t;

        fixed(point_id_t p, double v, fixed_coordinate c)
            : coordinate { nullptr, c }
            , value { nullptr, v }
            , point { nullptr, p }
        {}

        property<fixed_coordinate, TModel> coordinate;
        property<double, TModel> value;

        property<point_id_t, TModel> point;

        void bind(TModel& parent)
        {
            coordinate.bind(parent);
            value.bind(parent);
            point.bind(parent);
        }

        void reset_properties_updated()
        {
            coordinate.reset_updated();
            value.reset_updated();
            point.reset_updated();
        }
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const fixed<TVector2DIndexList, TModel>& f)
    {
        return os << (f.coordinate == fixed_coordinate::x ? "x=" : "y=") << f.value << " p:" << f.point;
    }
}

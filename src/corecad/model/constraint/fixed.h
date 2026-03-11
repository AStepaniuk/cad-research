#pragma once

#include "vector2d.h"
#include "property.h"

namespace corecad { namespace model { namespace constraint
{
    enum class fixed_coordinate { x, y };
    
    template<IsVector2D TVector, typename TModel>
    struct fixed
    {
        fixed(TVector::index_t p, double v, fixed_coordinate c)
            : coordinate { nullptr, c }
            , value { nullptr, v }
            , point { nullptr, p }
        {}

        property<fixed_coordinate, TModel> coordinate;
        property<double, TModel> value;

        property<typename TVector::index_t, TModel> point;

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
}}}
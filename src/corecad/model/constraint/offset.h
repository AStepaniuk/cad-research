#pragma once

#include "vector2d.h"
#include "property.h"

namespace corecad { namespace model { namespace constraint
{
    enum class offset_direction { horizontal, vertical };
    
    template<IsVector2D TVector, typename TModel>
    struct offset
    {
        offset(TVector::index_t f, TVector::index_t t, double o, offset_direction d)
            : direction { nullptr, d }
            , from { nullptr, f }
            , to { nullptr, t }
            , distance { nullptr, o }
        {}

        property<offset_direction, TModel> direction;

        property<typename TVector::index_t, TModel> from;
        property<typename TVector::index_t, TModel> to;

        property<double, TModel> distance;

        void bind(TModel& parent)
        {
            direction.bind(parent);
            from.bind(parent);
            to.bind(parent);
            distance.bind(parent);
        }
    };
}}}
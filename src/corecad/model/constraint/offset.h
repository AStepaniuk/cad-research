#pragma once

#include "vector2d.h"

namespace corecad { namespace model { namespace constraint
{
    enum class offset_direction { horizontal, vertical };
    
    template<IsVector2D TVector>
    struct offset
    {
        offset(TVector::index_t f, TVector::index_t t, double o, offset_direction d)
            : direction { d }
            , from { f }
            , to { t }
            , distance { o }
        {}

        offset_direction direction;

        TVector::index_t from;
        TVector::index_t to;

        double distance;
    };
}}}
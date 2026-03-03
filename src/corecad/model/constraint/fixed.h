#pragma once

#include "vector2d.h"

namespace corecad { namespace model { namespace constraint
{
    enum class fixed_coordinate { x, y };
    
    template<IsVector2D TVector>
    struct fixed
    {
        fixed(TVector::index_t p, double v, fixed_coordinate c)
            : coordinate { c }
            , value { v }
            , point { p }
        {}

        fixed_coordinate coordinate;
        double value;

        TVector::index_t point;
    };
}}}
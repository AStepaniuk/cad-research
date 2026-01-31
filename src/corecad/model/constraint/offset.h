#pragma once

#include "vector2d.h"

namespace corecad { namespace model { namespace constraint
{
    enum class offset_direction { horizontal, vertical };
    
    struct offset
    {
        offset(vector2d::index_t f, vector2d::index_t t, double o, offset_direction d)
            : direction { d }
            , from { f }
            , to { t }
            , distance { o }
        {}

        offset_direction direction;

        vector2d::index_t from;
        vector2d::index_t to;

        double distance;
    };
}}}
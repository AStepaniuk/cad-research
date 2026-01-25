#pragma once

#include "model_base.h"
#include "vector2d.h"

namespace corecad { namespace model
{
    enum class offset_direction { horizontal, vertical };
    
    struct parameter_offset : public model_base<parameter_offset>
    {
        offset_direction direction;

        vector2d::index_t from;
        vector2d::index_t to;

        double offset;
    };
}}
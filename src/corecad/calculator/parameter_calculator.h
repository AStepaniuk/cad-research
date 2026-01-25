#pragma once

#include "parameter_offset.h"
#include "vector2d.h"
#include "registry.h"

namespace corecad { namespace calculator
{
    class parameter_calculator
    {
    public:
        void recalculate_all(
            const corecad::model::registry<model::parameter_offset>& parameters,        
            corecad::model::registry<model::vector2d>& points
        );
    };
}}

#pragma once

#include <GCS.h>

#include "parameter_offset.h"
#include "vector2d.h"
#include "registry.h"

namespace corecad { namespace calculator
{
    class constraints_calculator
    {
    public:
        void recalculate_all(
            const corecad::model::registry<model::parameter_offset>& parameters,        
            corecad::model::registry<model::vector2d>& points
        );

    private:
        GCS::System m_sys;
    };
}}

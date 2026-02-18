#pragma once

#include "wall_calculator.h"
#include "constraints_calculator.h"

namespace gui { namespace editor {
    class calc_tools
    {
    public:
        calc_tools(domain::plan::model::floor& floor)
            : wall_calculator { floor }
        {}

        domain::plan::calculator::wall_calculator wall_calculator;

        corecad::calculator::constraints_calculator constraints_calculator;
    };
}}

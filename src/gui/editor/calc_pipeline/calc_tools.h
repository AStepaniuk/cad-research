#pragma once

#include "wall_calculator.h"
#include "constraints_calculator.h"
#include "wall.h"

namespace gui::editor {
    class calc_tools
    {
    public:
        calc_tools(domain::plan::model::shape::floor& floor)
            : wall_calculator { floor }
        {}

        domain::plan::calculator::wall_calculator wall_calculator;

        corecad::calculator::constraints_calculator<
            domain::plan::model::shape::wall_axis_point,
            domain::plan::model::shape::floor::constraint_t
        > constraints_calculator;
    };
}

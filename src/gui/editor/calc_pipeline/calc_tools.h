#pragma once

#include "wall_calculator.h"
#include "constraints_calculator.h"
#include "point_resolver.h"
#include "wall.h"

namespace gui::editor {
    class calc_tools
    {
    public:
        calc_tools(domain::plan::model::floor& floor)
            : wall_calculator { floor }
            , point_resolver { floor }
        {}

        domain::plan::calculator::wall_calculator wall_calculator;

        corecad::calculator::constraints_calculator<
            domain::plan::model::shape::wall_axis_point,
            domain::plan::model::floor::constraint_t
        > constraints_calculator;

        domain::plan::resolver::point_resolver point_resolver;
    };
}

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
            , constraint_calculator {
                floor.data().items<domain::plan::model::shape::wall_axis_point>(),
                floor.data().items<domain::plan::model::shape::wall_border_point>()
            }
            , point_resolver { floor }
        {}

        domain::plan::calculator::wall_calculator wall_calculator;

        corecad::calculator::constraints_calculator<domain::plan::model::floor::constraint_t> constraint_calculator;

        domain::plan::resolver::point_resolver point_resolver;
    };
}

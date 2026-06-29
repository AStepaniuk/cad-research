#pragma once

#include "wall_calculator.h"
#include "constraints_calculator.h"
#include "point_resolver.h"
#include "constraints_builder.h"
#include "wall.h"

namespace gui::editor {
    class calc_tools
    {
    public:
        using constraints_calculator_t = corecad::calculator::constraints_calculator<domain::plan::model::floor::constraint_t>;

        calc_tools(domain::plan::model::floor& floor)
            : _floor { floor }
            , _point_resolver { floor }
            , _wall_calculator { floor }
            , _constraints_builder { floor, _point_resolver }
            , _constraints_calculator {
                floor.data().items<domain::plan::model::shape::wall_axis_point>(),
                floor.data().items<domain::plan::model::shape::wall_border_point>()
            }
        {}

        void run_full_pipeline();

        domain::plan::resolver::point_resolver& point_resolver();
        domain::plan::calculator::wall_calculator wall_calculator();
        constraints_calculator_t& constraints_calculator();

    private:
        domain::plan::model::floor& _floor;

        domain::plan::resolver::point_resolver _point_resolver;

        domain::plan::calculator::wall_calculator _wall_calculator;

        domain::plan::calculator::constraints_builder _constraints_builder;

        constraints_calculator_t _constraints_calculator;
    };
}

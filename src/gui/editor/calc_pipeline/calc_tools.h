#pragma once

#include "floor_query.h"
#include "wall_calculator.h"
#include "constraints_calculator.h"
#include "constraints_builder.h"
#include "parameters_redundance_checker.h"
#include "wall.h"

namespace gui::editor
{
    class calc_tools
    {
    public:
        using constraints_calculator_t = corecad::calculator::constraints_calculator<
            domain::plan::model::floor::constraint_t,
            domain::plan::model::floor::data_t
        >;

        calc_tools(domain::plan::model::floor& floor)
            : _floor { floor }
            , _floor_query { floor }
            , _parameters_redundance_checker { _floor_query }
            , _wall_calculator { floor }
            , _constraints_builder { floor, _floor_query }
            , _constraints_calculator { floor.data() }
        {}

        void run_full_pipeline();

        domain::plan::calculator::floor_query& floor_query();
        domain::plan::calculator::parameters_redundance_checker& parameters_redundance_checker();
        domain::plan::calculator::wall_calculator& wall_calculator();
        domain::plan::calculator::constraints_builder& constraints_builder();
        constraints_calculator_t& constraints_calculator();

    private:
        domain::plan::model::floor& _floor;

        domain::plan::calculator::floor_query _floor_query;
        domain::plan::calculator::parameters_redundance_checker _parameters_redundance_checker;
        domain::plan::calculator::wall_calculator _wall_calculator;
        domain::plan::calculator::constraints_builder _constraints_builder;
        constraints_calculator_t _constraints_calculator;
    };
}

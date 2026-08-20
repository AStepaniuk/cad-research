#include "calc_tools.h"

using namespace gui::editor;
using namespace domain::plan::model;
using namespace domain::plan::calculator;

void calc_tools::run_full_pipeline()
{
    _wall_calculator.recalculate_all_walls();
    _constraints_builder.rebuild_all_constraints();
    _constraints_calculator.recalculate_all();
    _wall_calculator.recalculate_all_walls();
}

floor_query &calc_tools::floor_query()
{
    return _floor_query;
}

parameters_redundance_checker &calc_tools::parameters_redundance_checker()
{
    return _parameters_redundance_checker;
}

wall_calculator &calc_tools::wall_calculator()
{
    return _wall_calculator;
}

constraints_builder &gui::editor::calc_tools::constraints_builder()
{
    return _constraints_builder;
}

calc_tools::constraints_calculator_t& calc_tools::constraints_calculator()
{
    return _constraints_calculator;
}

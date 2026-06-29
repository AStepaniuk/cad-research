#include "calc_tools.h"

using namespace gui::editor;
using namespace domain::plan::model;
using namespace domain::plan::resolver;

void calc_tools::run_full_pipeline()
{
    _wall_calculator.recalculate_all_walls();
    _constraints_builder.rebuild_all_constraints();
    _constraints_calculator.recalculate_all(_floor.data().items<floor::constraint_t>());
}

point_resolver& calc_tools::point_resolver()
{
    return _point_resolver;
}

domain::plan::calculator::wall_calculator gui::editor::calc_tools::wall_calculator()
{
    return _wall_calculator;
}

calc_tools::constraints_calculator_t& calc_tools::constraints_calculator()
{
    return _constraints_calculator;
}

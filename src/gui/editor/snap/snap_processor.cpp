#include "snap_processor.h"

#include <ranges>
#include <vector>
#include <iostream>

using namespace gui::editor::snap;
using namespace gui::editor;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;
using namespace corecad::model;
using namespace corecad::calculator;

snap_processor::snap_processor(doc::document &doc, calc_tools& ct)
    : _document { doc }
    , _calc_tools { ct }
{
}

bool gui::editor::snap::snap_processor::process()
{
    if (!_document.active_handle)
    {
        return false;
    }

    while (!_document.active_wall_snaps.parameters().empty())
    {
        registry<floor::constraint_t> constraints;

        _calc_tools.constraints_builder().to_constraints(
            _document.active_wall_snaps.parameters(),
            constraints
        );

        const auto result = _calc_tools.constraints_calculator().recalculate_all(
            constraints
        );

        if (result == constraint_calculation_result::success)
        {
            return true;
        }

        _document.active_wall_snaps.ease();
    }
    
    return false;
}

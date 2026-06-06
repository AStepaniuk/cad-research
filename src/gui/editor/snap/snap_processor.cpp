#include "snap_processor.h"

#include <ranges>
#include <vector>
#include <iostream>

using namespace gui::editor::snap;
using namespace domain::plan::model::shape;
using namespace corecad::calculator;

snap_processor::snap_processor(doc::document &doc)
    : _document { doc }
{
}

bool gui::editor::snap::snap_processor::process()
{
    if (!_document.active_handle)
    {
        return false;
    }

    while (!_document.active_wall_snaps.constraints().empty())
    {
        const auto result = _constraint_calculator.recalculate_all(
            _document.active_wall_snaps.constraints(),
            _document.model.data().items<wall_axis_point>()
        );

        if (result == constraint_calculation_result::success)
        {
            return true;
        }

        _document.active_wall_snaps.ease();
    }
    
    return false;
}

#pragma once

#include "document.h"
#include "constraints_calculator.h"

#include <vector>
#include <utility>

namespace gui { namespace editor { namespace snap {
    class snap_processor
    {
    public:
        snap_processor(doc::document& doc);

        bool process();

    private:
        doc::document& _document;
        corecad::calculator::constraints_calculator<
            domain::plan::model::shape::wall_axis_point,
            domain::plan::model::shape::floor::constraint_t
        > _constraint_calculator;
    };
}}}

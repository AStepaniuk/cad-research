#pragma once

#include "vector2d.h"
#include "wall.h"

namespace gui { namespace editor { namespace handler {
    class i_mmove_wall_handler
    {
    public:
        virtual bool wall_move(
            float view_pos_x, float view_pos_y,
            domain::plan::model::wall_axis_point& model_pos
        ) = 0;

        virtual void apply() = 0;
    };
}}}

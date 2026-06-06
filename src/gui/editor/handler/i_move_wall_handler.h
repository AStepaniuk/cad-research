#pragma once

#include <optional>

#include "vector2d.h"
#include "wall.h"

namespace gui { namespace editor { namespace handler {
    class i_move_wall_handler
    {
    public:
        virtual bool wall_move(
            float view_pos_x, float view_pos_y,
            domain::plan::model::shape::wall_axis_point& model_pos
        ) = 0;

        virtual std::optional<domain::plan::model::shape::wall_axis_point::index_t> apply() = 0;
    };
}}}

#pragma once

#include <optional>

#include "vector2d.h"
#include "wall.h"
#include "handle_data.h"

namespace gui::editor::handler
{
    struct point_locator_replacement
    {
        domain::plan::model::shape::wall::index_t wid_from;
        domain::plan::model::shape::wall::index_t wid_to;
        domain::plan::model::shape::point_on_wall_axis_ptr wall_point;
    };
    
    struct post_apply_actions
    {
        std::optional<doc::handle_data> new_active_handle;
        std::optional<point_locator_replacement> pl_replacement;
    };

    class i_move_wall_handler
    {
    public:
        virtual bool wall_move(
            float view_pos_x, float view_pos_y,
            domain::plan::model::shape::wall_axis_point& model_pos
        ) = 0;

        virtual post_apply_actions apply() = 0;
    };
}

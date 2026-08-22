#pragma once

#include <optional>

#include "floor.h"
#include "collection.h"
#include "wall_snaps.h"
#include "parameter.h"
#include "handle_data.h"

namespace gui::doc
{
    struct document
    {
        domain::plan::model::floor model;

        // mouse hovered walls
        std::optional<domain::plan::model::shape::wall::index_t> hovered_wall_id;
        // mouse hovered wall handles
        std::optional<handle_data> hovered_handle;

        // selected walls
        corecad::model::collection<domain::plan::model::shape::wall> selected_walls;
        // selected handles
        corecad::model::collection<domain::plan::model::shape::wall_axis_point> selected_handles;
        
        // handle is currenly being moved
        std::optional<handle_data> active_handle;
        // snaps of the active wall handle
        wall_snaps active_wall_snaps { model.data() };
    };
}

#pragma once

#include <optional>

#include "floor.h"
#include "collection.h"
#include "wall_snaps.h"

namespace gui { namespace doc {
    struct document
    {
        domain::plan::model::shape::floor model;

        // mouse hovered walls
        std::optional<domain::plan::model::shape::wall::index_t> hovered_wall_id;
        // mouse hovered wall handles
        std::optional<domain::plan::model::shape::wall_axis_point::index_t> hovered_handle;

        // selected walls
        corecad::model::collection<domain::plan::model::shape::wall> selected_walls;
        // selected handles
        corecad::model::collection<domain::plan::model::shape::wall_axis_point> selected_handles;
        
        // handle is currenly being moved
        std::optional<domain::plan::model::shape::wall_axis_point::index_t> active_handle;
        // walls are currently being moved
        corecad::model::collection<domain::plan::model::shape::wall> active_walls;
        // snaps of the active wall handle
        wall_snaps active_wall_snaps { model.data().items<domain::plan::model::shape::wall_axis_point>() };
    };
}}

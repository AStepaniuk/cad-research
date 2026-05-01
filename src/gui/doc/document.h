#pragma once

#include <optional>

#include "floor.h"
#include "collection.h"

namespace gui { namespace doc {
    class document
    {
    public:
        domain::plan::model::floor model;

        // mouse hovered walls
        std::optional<domain::plan::model::wall::index_t> hovered_wall_id;
        // mouse hovered wall handles
        std::optional<domain::plan::model::wall_axis_point::index_t> hovered_handle;

        // selected walls
        corecad::model::collection<domain::plan::model::wall> selected_walls;
        // selected handles
        corecad::model::collection<domain::plan::model::wall_axis_point> selected_handles;
        
        // handle is currenly being moved
        std::optional<domain::plan::model::wall_axis_point::index_t> active_handle;
        // walls are currently being moved
        corecad::model::collection<domain::plan::model::wall> active_walls;
    };
}}

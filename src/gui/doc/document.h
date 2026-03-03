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
        corecad::model::collection<domain::plan::model::wall_axis_point> hovered_handles;

        // selected walls
        corecad::model::collection<domain::plan::model::wall> selected_walls;
        // handles are currenly being moved
        corecad::model::collection<domain::plan::model::wall_axis_point> active_handles;
        // walls are currently being moved
        corecad::model::collection<domain::plan::model::wall> active_walls;
    };
}}

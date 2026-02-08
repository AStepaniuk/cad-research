#pragma once

#include <optional>

#include "floor.h"
#include "collection.h"

namespace gui { namespace doc {
    class document
    {
    public:
        domain::plan::model::floor model;

        std::optional<domain::plan::model::wall::index_t> hovered_wall_id;
        corecad::model::collection<corecad::model::vector2d> hovered_handles;

        corecad::model::collection<domain::plan::model::wall> selected_walls;
        corecad::model::collection<corecad::model::vector2d> active_handles;
    };
}}

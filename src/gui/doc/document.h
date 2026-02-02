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

        corecad::model::collection<domain::plan::model::wall> selected_walls;
    };
}}

#pragma once

#include "i_move_wall_handler.h"

#include <optional>

#include "document.h"
#include "floor_view.h"

namespace gui { namespace editor { namespace handler {
    class wall_join_handler : public i_mmove_wall_handler
    {
        doc::document& _document;
        floor_view& _view;

        std::optional<corecad::model::vector2d::index_t> _joined_point_index;
        


    public:
        wall_join_handler(doc::document &doc, floor_view& v);

        bool wall_move(
            float view_pos_x, float view_pos_y,
            corecad::model::vector2d& model_pos
        ) override;
    };
}}}

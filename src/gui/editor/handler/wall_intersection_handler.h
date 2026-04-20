#pragma once

#include "i_move_wall_handler.h"

#include "document.h"
#include "floor_view.h"

namespace gui { namespace editor { namespace handler {
    class wall_intersection_handler : public i_mmove_wall_handler
    {
        doc::document& _document;
        floor_view& _view;

    public:
        wall_intersection_handler(doc::document &doc, floor_view& v);

        bool wall_move(float view_pos_x, float view_pos_y, domain::plan::model::wall_axis_point& model_pos) override;
        void apply() override;
    };
}}}

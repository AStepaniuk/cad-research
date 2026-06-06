#pragma once

#include "i_move_wall_handler.h"

#include "document.h"
#include "floor_view.h"
#include "snap_processor.h"

namespace gui::editor::handler {
    class wall_t_join_handler : public i_move_wall_handler
    {
        doc::document& _document;
        floor_view& _view;
        
        snap::snap_processor _wall_snap_processor;
        
        std::optional<domain::plan::model::shape::wall::index_t> _t_joint_wall;

    public:
        wall_t_join_handler(doc::document &doc, floor_view& v);

        bool wall_move(float view_pos_x, float view_pos_y, domain::plan::model::shape::wall_axis_point& model_pos) override;
        std::optional<domain::plan::model::shape::wall_axis_point::index_t> apply() override;
    };
}

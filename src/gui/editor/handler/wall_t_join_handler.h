#pragma once

#include "i_move_wall_handler.h"

#include "document.h"
#include "floor_view.h"

namespace gui { namespace editor { namespace handler {
    class wall_t_join_handler : public i_mmove_wall_handler
    {
        doc::document& _document;
        floor_view& _view;

        struct joint_data
        {
            domain::plan::model::wall_axis_point joint_point;
            domain::plan::model::wall::index_t joint_wall_index;
        };
        
        std::optional<joint_data> _t_joint_data;

    public:
        wall_t_join_handler(doc::document &doc, floor_view& v);

        bool wall_move(float view_pos_x, float view_pos_y, domain::plan::model::wall_axis_point& model_pos) override;
        std::optional<domain::plan::model::wall_axis_point::index_t> apply() override;
    };
}}}

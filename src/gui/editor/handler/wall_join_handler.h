#pragma once

#include "i_move_wall_handler.h"

#include "document.h"
#include "floor_view.h"

namespace gui { namespace editor { namespace handler {
    class wall_join_handler : public i_move_wall_handler
    {
        doc::document& _document;
        floor_view& _view;

        std::optional<doc::handle_data> _target_point_handle;

    public:
        wall_join_handler(doc::document &doc, floor_view& v);

        bool wall_move(float view_pos_x, float view_pos_y, domain::plan::model::shape::wall_axis_point& model_pos) override;
        std::optional<doc::handle_data> apply() override;
    };
}}}

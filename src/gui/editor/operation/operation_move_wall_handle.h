#pragma once

#include "i_operation.h"

#include <vector>

#include "document.h"
#include "floor_view.h"
#include "calc_tools.h"

namespace gui { namespace editor { namespace operation {
    class operation_move_wall_handle : public i_operation
    {
        doc::document& _document;
        floor_view& _view;
        calc_tools& _tools;

        // operation state
        std::vector<corecad::model::vector2d::index_t> _active_points;
        std::vector<corecad::model::vector2d> _initial_positions;

    public:
        operation_move_wall_handle(doc::document& doc, floor_view& v, calc_tools& t);

        void stop() override;
        action_handle_status handle_rollback() override;

        action_handle_status handle_mouse_move(float mx, float my) override;
        action_handle_status handle_left_mouse_click(float mx, float my) override;
    };
}}}

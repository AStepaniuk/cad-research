#pragma once

#include "i_operation.h"

#include <optional>

#include "document.h"
#include "floor_view.h"
#include "calc_tools.h"

#include "operation_move_wall_handle.h"

#include "vector2d.h"

namespace gui { namespace editor { namespace operation {
    class operation_add_wall : public i_operation
    {
        doc::document& _document;
        floor_view& _view;
        calc_tools& _tools;

        std::optional<corecad::model::vector2d::index_t> _current_point = std::nullopt;
        std::optional<domain::plan::model::wall::index_t> _current_wall = std::nullopt;

        operation_move_wall_handle _sub_operation_move_handle;

    public:
        operation_add_wall(doc::document& doc, floor_view& v, calc_tools& t);

        void start() override;
        void stop() override;

        void cancel() override;
        action_handle_status rollback() override;

        action_handle_status mouse_move(float mx, float my) override;
        action_handle_status left_mouse_click(float mx, float my) override;
    };
}}}

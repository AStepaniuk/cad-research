#pragma once

#include "i_operation.h"

#include <optional>

#include "document.h"
#include "floor_view.h"
#include "calc_tools.h"
#include "attribute_service.h"
#include "root_ui_dispatcher.h"

#include "operation_move_wall_handle.h"

namespace gui::editor::operation
{
    class operation_add_wall : public i_operation
    {
        doc::document& _document;
        floor_view& _view;
        calc_tools& _tools;
        attribute::attribute_service& _attribute_service;
        ui_dispatcher::root_ui_dispatcher& _ui_dispatcher;

        std::optional<domain::plan::model::shape::wall_axis_point::index_t> _current_point = std::nullopt;

        operation_move_wall_handle _sub_operation_move_handle;

        corecad::model::length_mm_t _last_thickness = 400.0 * corecad::model::mm;

    public:
        operation_add_wall(
            doc::document& doc,
            floor_view& v,
            calc_tools& t,
            attribute::attribute_service& as,
            ui_dispatcher::root_ui_dispatcher& ui_dispatcher
        );

        void start() override;
        void stop() override;

        action_handle_status mouse_move(float mx, float my) override;
        action_handle_status left_mouse_click(float mx, float my) override;

        action_handle_status execute_instruction(const cmd_parser::instruction& instruction) override;
    };
}

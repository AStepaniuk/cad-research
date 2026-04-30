#pragma once

#include "i_operation.h"

#include <vector>
#include <string>

#include "document.h"
#include "floor_view.h"
#include "calc_tools.h"
#include "handler/wall_join_handler.h"
#include "handler/wall_t_join_handler.h"

namespace gui { namespace editor { namespace operation {
    class operation_move_wall_handle : public i_operation
    {
        doc::document& _document;
        floor_view& _view;
        calc_tools& _tools;

        handler::wall_join_handler _wall_join_handler;
        handler::wall_t_join_handler _wall_t_join_handler;

        std::vector<handler::i_mmove_wall_handler*> _move_wall_handlers;
        handler::i_mmove_wall_handler* _last_worked_move_wall_handler = nullptr;

        std::string _commit_message;
        bool _do_commit_on_click = true;

    public:
        operation_move_wall_handle(doc::document& doc, floor_view& v, calc_tools& t, std::string commit_msg);

        void start() override;
        void stop() override;

        action_handle_status mouse_move(float mx, float my) override;
        action_handle_status left_mouse_click(float mx, float my) override;

        void enable_commit_on_click();
        void disable_commit_on_click();
    };
}}}

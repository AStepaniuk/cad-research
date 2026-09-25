#include "operation_idle.h"

#include <operation_move_wall_handle.h>

using namespace gui::doc;
using namespace gui::editor::operation;

operation_idle::operation_idle(doc::document &doc, floor_view &v, calc_tools &t, ui_dispatcher::root_ui_dispatcher& ui_dispatcher)
    : _document { doc }
    , _view { v }
    , _tools { t }
    , _ui_dispatcher { ui_dispatcher }
    , _sub_operation_move_wall { doc, v, t, "Move wall" }
{
}

void gui::editor::operation::operation_idle::start()
{
}

void operation_idle::stop()
{
    if (_sub_operation)
    {
        _sub_operation->stop();
    }

    _document.selected_walls.clear();
    _document.hovered_wall_id = std::nullopt;
    
    _document.hovered_handle = std::nullopt;

    _ui_dispatcher.wall_context_panel.show = false;
}

action_handle_status operation_idle::mouse_move(float mx, float my)
{
    if (_sub_operation)
    {
        return _sub_operation->mouse_move(mx, my);
    } 
    else
    {
        auto hovered_handles = _view.get_handles(mx, my);

        if (hovered_handles.empty())
        {
            _document.hovered_wall_id = _view.get_wall(mx, my);
            _document.hovered_handle = std::nullopt;
        }
        else
        {
            _document.hovered_wall_id = std::nullopt;
            _document.hovered_handle = handle_data { hovered_handles[0] };
        }

        return action_handle_status::operation_continues;
    }
}

action_handle_status gui::editor::operation::operation_idle::left_mouse_click(float mx, float my)
{
    if (_sub_operation)
    {
        auto res = _sub_operation->left_mouse_click(mx, my);
        if (res == action_handle_status::operation_finished)
        {
            _sub_operation->stop();
            _sub_operation = nullptr;
        }

        return action_handle_status::operation_continues;
    } 
    else
    {
        if (_document.hovered_handle)
        {
            // start move wall handle sub-operation

            _sub_operation = &_sub_operation_move_wall;
            _sub_operation->start();

            return action_handle_status::operation_continues;
        }
        else
        {
            auto wall_id = _document.hovered_wall_id;
            if (wall_id)
            {
                // wall click occurred
                if (_document.selected_walls.contains(wall_id.value()))
                {
                    _document.selected_walls.remove(wall_id.value());

                    if (_document.selected_walls.empty())
                    {
                        _ui_dispatcher.wall_context_panel.show = false;
                    }
                }
                else
                {
                    _document.selected_walls.put(wall_id.value());

                    if (_document.selected_walls.size() == 1)
                    {
                        _ui_dispatcher.wall_context_panel.show = true;
                    }
                }

                return action_handle_status::operation_continues;
            }
            else
            {
                return action_handle_status::unhandled;
            }
        }
    }
}

action_handle_status operation_idle::execute_instruction(const cmd_parser::instruction &instruction)
{
    if (_sub_operation)
    {
        return _sub_operation->execute_instruction(instruction);
    }
    else
    {
        return action_handle_status::unhandled;
    }
}

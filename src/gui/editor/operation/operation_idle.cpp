#include "operation_idle.h"

#include <operation/operation_move_wall_handle.h>

using namespace gui::editor::operation;

operation_idle::operation_idle(doc::document &doc, floor_view &v, calc_tools &t)
    : _document { doc }
    , _view { v }
    , _tools { t }
{
}

void operation_idle::stop()
{
    if (sub_operation)
    {
        sub_operation->stop();
    }
}

action_handle_status operation_idle::handle_rollback()
{
    if (sub_operation)
    {
        auto res = sub_operation->handle_rollback();

        if (res == action_handle_status::operation_finished)
        {
            sub_operation.reset();
        }
    }

    return action_handle_status::operation_finished;
}

action_handle_status operation_idle::handle_mouse_move(float mx, float my)
{
    if (sub_operation)
    {
        return sub_operation->handle_mouse_move(mx, my);
    } 
    else
    {
        auto hovered_handles = _view.get_handles(mx, my);

        if (hovered_handles.empty())
        {
            _document.hovered_wall_id = _view.get_wall(mx, my);
        }
        else
        {
            _document.hovered_wall_id = std::nullopt;
        }
        _document.hovered_handles.clear();
        _document.hovered_handles.put(std::move(hovered_handles));

        return action_handle_status::operation_continues;
    }
}

action_handle_status gui::editor::operation::operation_idle::handle_left_mouse_click(float mx, float my)
{
    if (sub_operation)
    {
        auto res = sub_operation->handle_left_mouse_click(mx, my);
        if (res == action_handle_status::operation_finished)
        {
            sub_operation.reset();
        }

        return action_handle_status::operation_continues;
    } 
    else
    {
        if (!_document.hovered_handles.empty())
        {
            // start move wall handle sub-operation

            sub_operation = std::make_unique<operation_move_wall_handle>(_document, _view, _tools);

            return action_handle_status::operation_continues;
        }
        else
        {
            auto wall_id = _document.hovered_wall_id;
            if (wall_id)
            {
                if (_document.selected_walls.contains(wall_id.value()))
                {
                    _document.selected_walls.remove(wall_id.value());
                }
                else
                {
                    _document.selected_walls.put(wall_id.value());
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

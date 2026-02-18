#include "operation_add_wall.h"

#include <iostream>

using namespace gui::editor::operation;
using namespace corecad::model;

operation_add_wall::operation_add_wall(doc::document &doc, floor_view &v, calc_tools &t)
    : _document { doc }
    , _view { v }
    , _tools { t }
    , _sub_operation_move_handle { doc, v, t }
{
}

void operation_add_wall::start()
{
    _current_point = _document.model.points().make(0.0, 0.0);
    _current_wall = std::nullopt;

    _document.hovered_handles.clear();
    _document.hovered_handles.put(_current_point.value());

    _sub_operation_move_handle.start();
}

void operation_add_wall::stop()
{
    _sub_operation_move_handle.stop();

    if (_current_wall)
    {
        _document.model.walls().erase(_current_wall.value());
        _tools.wall_calculator.recalculate_all_walls(_document.model);

        _current_wall = std::nullopt;
    }
    _document.selected_walls.clear();

    _document.model.points().erase(_current_point.value());
    _current_point = std::nullopt;
}

void gui::editor::operation::operation_add_wall::cancel()
{
    _sub_operation_move_handle.cancel();

    if (_current_wall)
    {
        _document.model.walls().erase(_current_wall.value());
        _tools.wall_calculator.recalculate_all_walls(_document.model);
        
        _current_wall = std::nullopt;
    }
    _document.selected_walls.clear();

    _document.model.points().erase(_current_point.value());
    _current_point = std::nullopt;

}

action_handle_status operation_add_wall::rollback()
{
    _document.model.points().erase(_current_point.value());
    _current_point = std::nullopt;

    return action_handle_status::operation_finished;
}

action_handle_status operation_add_wall::mouse_move(float mx, float my)
{
    return _sub_operation_move_handle.mouse_move(mx, my);
}

action_handle_status operation_add_wall::left_mouse_click(float mx, float my)
{
    _sub_operation_move_handle.stop();
    
    auto m_model = _view.to_model(mx, my);
    auto next_index = _document.model.points().make(m_model.x, m_model.y);
 
    auto wall_index = _document.model.walls().make(_current_point.value(), next_index, 400.0);
    _current_wall = wall_index;

    _document.selected_walls.clear();
    _document.selected_walls.put(wall_index);
    _tools.wall_calculator.recalculate_all_walls(_document.model);
    _tools.constraints_calculator.recalculate_all(_document.model.parameters(), _document.model.points());

    _current_point = next_index;

    _document.hovered_handles.clear();
    _document.hovered_handles.put(_current_point.value());
    _sub_operation_move_handle.start();

    return action_handle_status::operation_continues;
}

#include "operation_add_wall.h"

#include <iostream>

using namespace gui::editor::operation;
using namespace corecad::model;
using namespace domain::plan::model;

operation_add_wall::operation_add_wall(doc::document &doc, floor_view &v, calc_tools &t)
    : _document { doc }
    , _view { v }
    , _tools { t }
    , _sub_operation_move_handle { doc, v, t }
{
}

void operation_add_wall::start()
{
    _current_point = std::make_optional(_document.model.data().make<wall_axis_point>(0.0, 0.0));
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
        _document.model.data().erase(_current_wall.value());
        _tools.wall_calculator.recalculate_all_walls();

        _current_wall = std::nullopt;
    }
    _document.selected_walls.clear();
    _document.hovered_handles.clear();

    _document.model.data().erase(_current_point.value());
    _current_point = std::nullopt;
}

action_handle_status operation_add_wall::mouse_move(float mx, float my)
{
    return _sub_operation_move_handle.mouse_move(mx, my);
}

action_handle_status operation_add_wall::left_mouse_click(float mx, float my)
{
    auto sub_result = _sub_operation_move_handle.left_mouse_click(mx, my);
    if (sub_result != action_handle_status::operation_finished)
    {
        return action_handle_status::operation_continues;
    }

    _sub_operation_move_handle.stop();
   
    auto m_model = _view.to_model(mx, my);
    auto next_index = _document.model.data().make<wall_axis_point>(m_model.x, m_model.y);

    if (*(_document.hovered_handles.begin()) != _current_point.value())
    {
        _document.model.data().erase(_current_point.value());
        _current_point = *(_document.hovered_handles.begin());
    }

    auto wall_index = _document.model.data().make<wall>(_current_point.value(), next_index, 400.0);
    _current_wall = wall_index;

    _document.selected_walls.clear();
    _document.selected_walls.put(wall_index);
    _tools.wall_calculator.recalculate_all_walls();
    _tools.constraints_calculator.recalculate_all(
        _document.model.data().items<floor::constraint_t>(),
        _document.model.data().items<wall_axis_point>()
    );

    _current_point = next_index;

    _document.hovered_handles.clear();
    _document.hovered_handles.put(_current_point.value());
    _sub_operation_move_handle.start();

    return action_handle_status::operation_continues;
}

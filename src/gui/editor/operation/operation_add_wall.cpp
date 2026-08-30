#include "operation_add_wall.h"

#include <iostream>

using namespace gui::doc;
using namespace gui::editor::operation;
using namespace corecad::model;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;

operation_add_wall::operation_add_wall(doc::document &doc, floor_view &v, calc_tools &t)
    : _document { doc }
    , _view { v }
    , _tools { t }
    , _sub_operation_move_handle { doc, v, t, "" }
{
    _sub_operation_move_handle.disable_commit_on_click();
}

void operation_add_wall::start()
{
    _current_point = _document.model.data().make<wall_axis_point>(0.0, 0.0);

    _document.hovered_handle = handle_data { _current_point.value() };

    _sub_operation_move_handle.start();
}

void operation_add_wall::stop()
{
    _sub_operation_move_handle.stop();

    _document.selected_walls.clear();
    _document.hovered_handle = std::nullopt;

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
   
    if (!_document.hovered_handle)
    {
        // something went unexpected
        return action_handle_status::operation_finished;
    }

    auto hhid = _document.hovered_handle->handle_id_of_type<wall_axis_point>();
    if (!hhid)
    {
        // something went unexpected
        return action_handle_status::operation_finished;
    }

    if (hhid != _current_point.value())
    {
        _document.model.data().erase(_current_point.value());
        _current_point = hhid;
    }

    _document.model.history().commit("Add wall");

    auto m_model = _view.to_model(mx, my);
    auto next_index = _document.model.data().make<wall_axis_point>(m_model.x, m_model.y);
    
    const auto axis_index = _document.model.data().make<wall_axis_line>(_current_point.value(), next_index);
    auto wall_index = _document.model.data().make<wall>(axis_index, 400.0);

    _document.selected_walls.clear();
    _document.selected_walls.put(wall_index);
    _tools.run_full_pipeline();

    _current_point = next_index;

    _document.hovered_handle = handle_data { _current_point.value() };

    _sub_operation_move_handle.start();

    return action_handle_status::operation_continues;
}

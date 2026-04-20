#include "operation_move_wall_handle.h"

#include <algorithm>
#include <ranges>

using namespace gui::editor::operation;
using namespace domain::plan::model;

operation_move_wall_handle::operation_move_wall_handle(doc::document &doc, floor_view& v, calc_tools& t)
    : _document { doc }
    , _view { v }
    , _tools { t }
    , _wall_join_handler { _document, _view }
    , _move_wall_handlers { &_wall_join_handler }
{
}

void operation_move_wall_handle::start()
{
    _document.active_handles.clear();
    _document.active_handles.put(_document.hovered_handles);

    _document.hovered_handles.clear();

    _document.active_walls.clear();
    _document.active_walls.put(
        _document.model.data().items<wall>()
            | std::views::transform([] (const auto& p) { return p.second; })
            | std::views::filter([this](const auto& w) {
                return std::ranges::any_of(
                    _document.active_handles, [&w](const auto& h) { return w.start == h || w.end == h; }
                );
            })
            | std::views::transform([] (const auto& w) { return w.index; })
    );
}

void operation_move_wall_handle::stop()
{
    _document.active_walls.clear();

    _document.hovered_handles.put(_document.active_handles);
    _document.active_handles.clear();
}

action_handle_status operation_move_wall_handle::mouse_move(float mx, float my)
{
    auto model_pos = wall_axis_point { _view.to_model(mx, my) };

    // check if model pos is applicable to any handler
    _last_worked_move_wall_handler = nullptr;
    for (auto handler : _move_wall_handlers)
    {
        if (handler->wall_move(mx, my, model_pos))
        {
            _last_worked_move_wall_handler = handler;
            break;
        }
    }

    // spply model pos to all active points
    for (const auto pid : _document.active_handles)
    {
        auto& p = _document.model.data().get(pid);
        p.x = model_pos.x;
        p.y = model_pos.y;
    }

    // update model
    _tools.constraints_calculator.recalculate_all(
        _document.model.data().items<floor::constraint_t>(),
        _document.model.data().items<wall_axis_point>()
    );
    _tools.wall_calculator.recalculate_all_walls();

    return action_handle_status::operation_continues;
}

action_handle_status operation_move_wall_handle::left_mouse_click(float mx, float my)
{
    if (_last_worked_move_wall_handler)
    {
        _last_worked_move_wall_handler->apply();
        _last_worked_move_wall_handler = nullptr;

        _tools.constraints_calculator.recalculate_all(
            _document.model.data().items<floor::constraint_t>(),
            _document.model.data().items<wall_axis_point>()
        );
        _tools.wall_calculator.recalculate_all_walls();
    }

    _document.hovered_handles.clear();
    _document.hovered_handles.put(_document.active_handles);

    _document.active_handles.clear();

    return action_handle_status::operation_finished;
}

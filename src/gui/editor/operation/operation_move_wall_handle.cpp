#include "operation_move_wall_handle.h"

#include <algorithm>
#include <ranges>

using namespace gui::editor::operation;

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

    _active_points = _document.active_handles | std::ranges::to<std::vector>();
    _initial_positions = _active_points
        | std::views::transform([this](auto pid) { return _document.model.points().get(pid); })
        | std::ranges::to<std::vector>();

    _document.active_walls.clear();
    _document.active_walls.put(
        _document.model.walls()
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

    // TODO: implement commit move operation
}

void gui::editor::operation::operation_move_wall_handle::cancel()
{
    for (size_t i = 0; i < _active_points.size(); ++i)
    {
        const auto& pid = _active_points[i];
        const auto& initial_pos = _initial_positions[i];

        auto& point = _document.model.points().get(pid);
        point.x = initial_pos.x;
        point.y = initial_pos.y;
    }

    _initial_positions.clear();
    _active_points.clear();
}

action_handle_status operation_move_wall_handle::rollback()
{
    // TODO: implement revert move operation
    return action_handle_status::operation_finished;
}

action_handle_status operation_move_wall_handle::mouse_move(float mx, float my)
{
    auto model_pos = _view.to_model(mx, my);

    // check if model pos is applicable to any handler
    for (auto handler : _move_wall_handlers)
    {
        if (handler->wall_move(mx, my, model_pos))
        {
            break;
        }
    }

    // spply model pos to all active points
    for (const auto pid : _active_points)
    {
        auto& p = _document.model.points().get(pid);
        p.x = model_pos.x;
        p.y = model_pos.y;
    }

    // update model
    _tools.constraints_calculator.recalculate_all(_document.model.parameters(), _document.model.points());
    _tools.wall_calculator.recalculate_all_walls();

    return action_handle_status::operation_continues;
}

action_handle_status operation_move_wall_handle::left_mouse_click(float mx, float my)
{
    _document.hovered_handles.clear();
    _document.hovered_handles.put(_document.active_handles);

    _document.active_handles.clear();

    return action_handle_status::operation_finished;
}

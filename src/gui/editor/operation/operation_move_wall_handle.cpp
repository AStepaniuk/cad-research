#include "operation_move_wall_handle.h"

#include <algorithm>
#include <ranges>

using namespace gui::editor::operation;
using namespace domain::plan::model;

operation_move_wall_handle::operation_move_wall_handle(doc::document &doc, floor_view &v, calc_tools &t, std::string commit_msg)
    : _document{doc}
    , _view{v}
    , _tools{t}
    , _wall_join_handler{_document, _view}
    , _wall_t_join_handler{_document, _view}
    , _move_wall_handlers{&_wall_join_handler, &_wall_t_join_handler}
    , _commit_message{std::move(commit_msg)}
{
}

void operation_move_wall_handle::start()
{
    _document.active_handle = _document.hovered_handle;
    _document.hovered_handle = std::nullopt;

    _document.active_walls.clear();
    _document.active_walls.put(
        _document.model.data().items<wall>()
            | std::views::transform([] (const auto& p) { return p.second; })
            | std::views::filter([this](const auto& w) {
                return w.start == _document.active_handle || w.end == _document.active_handle;
            })
            | std::views::transform([] (const auto& w) { return w.index; })
    );
}

void operation_move_wall_handle::stop()
{
    _document.active_walls.clear();

    _document.hovered_handle = _document.active_handle;
    _document.active_handle = std::nullopt;
}

action_handle_status operation_move_wall_handle::mouse_move(float mx, float my)
{
    if (!_document.active_handle)
    {
        return action_handle_status::unhandled;
    }

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

    // spply model pos to all active point
    auto& p = _document.model.data().get(_document.active_handle.value());
    p.x = model_pos.x;
    p.y = model_pos.y;

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
        const auto worked_point_id = _last_worked_move_wall_handler->apply();
        _last_worked_move_wall_handler = nullptr;

        if (worked_point_id)
        {
            _document.active_handle = worked_point_id.value();
        }

        _tools.constraints_calculator.recalculate_all(
            _document.model.data().items<floor::constraint_t>(),
            _document.model.data().items<wall_axis_point>()
        );
        _tools.wall_calculator.recalculate_all_walls();
    }

    _document.hovered_handle = _document.active_handle;

    if (_do_commit_on_click)
    {
        _document.model.history().commit(_commit_message);
    }

    return action_handle_status::operation_finished;
}

void operation_move_wall_handle::enable_commit_on_click()
{
    _do_commit_on_click = true;
}

void operation_move_wall_handle::disable_commit_on_click()
{
    _do_commit_on_click = false;
}

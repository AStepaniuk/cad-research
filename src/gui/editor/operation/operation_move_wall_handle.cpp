#include "operation_move_wall_handle.h"

#include <algorithm>
#include <ranges>

using namespace gui::editor::operation;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;

operation_move_wall_handle::operation_move_wall_handle(doc::document &doc, floor_view &v, calc_tools &t, std::string commit_msg)
    : _document{doc}
    , _view{v}
    , _tools{t}
    , _wall_snap_processor{doc, t}
    , _wall_join_handler{_document, _view}
    , _wall_t_join_handler{_document, _view, t}
    , _move_wall_handlers{&_wall_join_handler, &_wall_t_join_handler}
    , _vh_snap_builder{_document, _view}
    , _snap_builders{&_vh_snap_builder}
    , _commit_message{std::move(commit_msg)}
{
}

void operation_move_wall_handle::start()
{
    _document.active_handle = _document.hovered_handle;
    _document.hovered_handle = std::nullopt;
}

void operation_move_wall_handle::stop()
{
    _document.hovered_handle = _document.active_handle;
    _document.active_handle = std::nullopt;

    _document.active_wall_snaps.clear();
}

action_handle_status operation_move_wall_handle::mouse_move(float mx, float my)
{
    if (!_document.active_handle)
    {
        return action_handle_status::unhandled;
    }

    auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!ahid)
    {
        return action_handle_status::unhandled;
    }

    auto model_pos = wall_axis_point { _view.to_model(mx, my) };

    // apply model pos to active point
    auto& active_point = _document.model.data().get(ahid);
    active_point.x = model_pos.x;
    active_point.y = model_pos.y;

    _document.active_wall_snaps.clear();
    for (auto wall_snap_builder : _snap_builders)
    {
        wall_snap_builder->calculate_snaps(mx, my);
    }
    std::cout << std::endl << "Snap-driven parameters:" << std::endl;
    std::cout << _document.active_wall_snaps.parameters();

    // check if model pos is applicable to any handler
    _last_worked_move_wall_handler = nullptr;
    for (auto handler : _move_wall_handlers)
    {
        if (handler->wall_move(mx, my, model_pos))
        {
            _last_worked_move_wall_handler = handler;

            // re-apply model pos to active point
            active_point.x = model_pos.x;
            active_point.y = model_pos.y;
            break;
        }
    }
    std::cout << std::endl << "Handler-enriched parameters:" << std::endl;
    std::cout << _document.active_wall_snaps.parameters();

    _wall_snap_processor.process();
    std::cout << std::endl << "Cleared parameters:" << std::endl;
    std::cout << _document.active_wall_snaps.parameters();

    // update model
    _tools.run_full_pipeline();

    return action_handle_status::operation_continues;
}

action_handle_status operation_move_wall_handle::left_mouse_click(float mx, float my)
{
    bool needs_recalculation = false;
    handler::post_apply_actions post_actions;

    if (_last_worked_move_wall_handler)
    {
        post_actions = _last_worked_move_wall_handler->apply();
        _last_worked_move_wall_handler = nullptr;

        if (post_actions.new_active_handle)
        {
            _document.active_handle = post_actions.new_active_handle.value();
        }

        needs_recalculation = true;
    }

    _document.hovered_handle = _document.active_handle;

    std::cout << std::endl << "Applying parameters:" << std::endl;
    std::cout << _document.active_wall_snaps.parameters();

    if (!_document.active_wall_snaps.parameters().empty())
    {
        for (const auto& p : _document.active_wall_snaps.parameters())
        {
            if (!_tools.parameters_redundance_checker().is_parameter_redundant(p.second))
            {
                _document.model.data().put(p.second);
            }
        }

        needs_recalculation = true;

        std::cout << std::endl << "Parameters updated:" << std::endl;
        std::cout << _document.model.data().items<parameter::parameter>();
    }

    if(needs_recalculation)
    {
        _tools.run_full_pipeline();
    } 

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

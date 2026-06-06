#include "wall_join_handler.h"

#include <ranges>
#include <iostream>

using namespace gui::editor::handler;
using namespace domain::plan::model::shape;
using namespace corecad::model;

wall_join_handler::wall_join_handler(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
{
}

bool wall_join_handler::wall_move(
    float view_pos_x, float view_pos_y,
    wall_axis_point& model_pos
)
{
    const auto tol = _view.model_interaction_tolerance();

    for (auto& p : _document.model.data().items<wall>())
    {
        if (std::ranges::any_of(_document.active_walls, [&p](const auto& w_index) { return w_index == p.second.index; }))
        {
            continue;
        }

        const auto& a = _document.model.data().get(p.second.axis);

        const auto& sp = _document.model.data().get(a.s);
    
        if (model_pos.x > sp.x - tol.x && model_pos.x < sp.x + tol.x
        && model_pos.y > sp.y - tol.y && model_pos.y < sp.y + tol.y)
        {
            _target_point_index = sp.index;

            model_pos.x = sp.x;
            model_pos.y = sp.y;

            _document.active_wall_snaps.clear();

            return true;
        }

        const auto& ep = _document.model.data().get(a.e);
    
        if (model_pos.x > ep.x - tol.x && model_pos.x < ep.x + tol.x
        && model_pos.y > ep.y - tol.y && model_pos.y < ep.y + tol.y)
        {
            _target_point_index = ep.index;

            model_pos.x = ep.x;
            model_pos.y = ep.y;

            _document.active_wall_snaps.clear();

            return true;
        }
    }

    _target_point_index = std::nullopt;

    return false;
}

std::optional<wall_axis_point::index_t> wall_join_handler::apply()
{
    if (!_target_point_index || !_document.active_handle)
    {
        return std::nullopt;
    }

    for (const auto wid : _document.active_walls)
    {
        auto& wall = _document.model.data().get(wid);
        auto& axis = _document.model.data().get(wall.axis);

        if (axis.s == _document.active_handle.value())
        {
            axis.s = _target_point_index.value();
        }
        else
        {
            axis.e = _target_point_index.value();
        }
    }

    return _target_point_index;
}

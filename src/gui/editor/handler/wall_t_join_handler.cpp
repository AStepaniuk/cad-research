#include "wall_t_join_handler.h"

#include <iostream>
#include <cmath>

using namespace gui::editor::handler;
using namespace domain::plan::model;
using namespace corecad::model;

wall_t_join_handler::wall_t_join_handler(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
{
}

bool wall_t_join_handler::wall_move(
    float view_pos_x, float view_pos_y,
    wall_axis_point& model_pos
)
{
    const auto tol = _view.model_interaction_tolerance();
    const double tol2 = tol.x * tol.y;

    for (auto& p : _document.model.data().items<wall>())
    {
        if (std::ranges::any_of(_document.active_walls, [&p](const auto& w_index) { return w_index == p.second.index; }))
        {
            continue;
        }

        const auto& sp = _document.model.data().get(p.second.start);
        const auto& ep = _document.model.data().get(p.second.end);

        const auto min_x = (sp.x > ep.x ? ep.x : sp.x) - tol.x;
        const auto max_x = (sp.x > ep.x ? sp.x : ep.x) + tol.x;
        const auto min_y = (sp.y > ep.y ? ep.y : sp.y) - tol.y;
        const auto max_y = (sp.y > ep.y ? sp.y : ep.y) + tol.y;
    
        if (model_pos.x < min_x || model_pos.x > max_x || model_pos.y < min_y || model_pos.y > max_y)
        {
            continue;
        }

        const double wdx = ep.x - sp.x;
        const double wdy = ep.y - sp.y;

        if (wdx == 0.0 && wdy == 0.0)
        {
            continue;
        }

        const double pdx = sp.x - model_pos.x;
        const double pdy = sp.y - model_pos.y;

        const double den2 = 1 / (wdx*wdx + wdy*wdy);
        const double nom = wdx*pdy - wdy*pdx;
        const double nom2 = nom*nom;

        const double d2 = nom2 * den2;

        if (d2 > tol2)
        {
            continue;
        }

        const double t = -(wdx*pdx + wdy*pdy) * den2;

        if (t < 0.0 || t > 1.0)
        {
            continue;
        }

        model_pos = wall_axis_point { sp.x + t*wdx, sp.y + t*wdy };
        _t_joint_data = joint_data { model_pos, p.first };

        return true;
    }

    _t_joint_data = std::nullopt;
    return false;
}

std::optional<domain::plan::model::wall_axis_point::index_t> wall_t_join_handler::apply()
{
    if (!_t_joint_data)
    {
        return std::nullopt;
    }

    // register new point
    const auto npid = _document.model.data().put(_t_joint_data->joint_point);

    // split wall into 2 walls
    auto& w = _document.model.data().get(_t_joint_data->joint_wall_index);

    const auto epid = w.end.val();
    w.end = npid;

    _document.model.data().make<wall>(npid, epid, w.width);

    // add constraing to keep both walls aligned
    _document.model.data().put(floor::constraint_t::create<floor::aligned_wall_axis_point_t>(w.start, npid, epid));

    // update active walls points
    for (const auto wid : _document.active_walls)
    {
        auto& wall = _document.model.data().get(wid);

        if (std::ranges::any_of(_document.active_handles, [&wall](const auto& h) { return wall.start == h; }))
        {
            wall.start = npid;
        }
        else
        {
            wall.end = npid;
        }
    }

    return npid;
}

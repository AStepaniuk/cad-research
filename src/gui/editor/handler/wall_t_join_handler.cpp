#include "wall_t_join_handler.h"

#include <iostream>
#include <cmath>

#include "views_take_constraints.h"

using namespace gui::editor::handler;
using namespace domain::plan::model::shape;
using namespace corecad::model;
using namespace corecad::model::constraint;

wall_t_join_handler::wall_t_join_handler(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
    , _wall_snap_processor { doc }
{
}

bool wall_t_join_handler::wall_move(
    float view_pos_x, float view_pos_y,
    wall_axis_point& model_pos
)
{
    if (!_document.active_handle)
    {
        return false;
    }

    const auto tol = _view.model_interaction_tolerance();
    const double tol2 = tol.x * tol.y;

    for (auto& p : _document.model.data().items<wall>())
    {
        if (std::ranges::any_of(_document.active_walls, [&p](const auto& w_index) { return w_index == p.second.index; }))
        {
            continue;
        }

        const auto& axis = _document.model.data().get(p.second.axis);

        const auto& sp = _document.model.data().get(axis.s);
        const auto& ep = _document.model.data().get(axis.e);

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

        // check if wall already has h/v 0-offset snap
        bool existing_snap = false;

        auto offsets = _document.active_wall_snaps.constraints()
            | views::take_constraints<floor::offset_wall_axis_point_t>();
        for (const auto& o : offsets)
        {
            if (o.distance == 0.0 && (o.from == sp.index || o.from == ep.index || o.to == sp.index || o.to == ep.index))
            {
                existing_snap = true;
                break;
            }
        }

        if (!existing_snap)
        {
            _document.active_wall_snaps.add(
                floor::constraint_t::create<floor::aligned_wall_axis_point_t>(sp.index, _document.active_handle.value(), ep.index),
                0.0,
                sp.index, ep.index
            );
        }

        _wall_snap_processor.process();

        _t_joint_wall = p.first;

        const auto& ap = _document.model.data().get(_document.active_handle.value());
        model_pos.x = ap.x;
        model_pos.y = ap.y;

        return true;
    }

    _t_joint_wall = std::nullopt;
    return false;
}

std::optional<wall_axis_point::index_t> wall_t_join_handler::apply()
{
    if (!_t_joint_wall || !_document.active_handle)
    {
        return std::nullopt;
    }

    // split wall into 2 walls
    const auto& w = _document.model.data().get(_t_joint_wall.value());
    auto& a = _document.model.data().get(w.axis);

    const auto epid = a.e.val();
    a.e = _document.active_handle.value();

    const auto aid = _document.model.data().make<wall_axis_line>(_document.active_handle.value(), epid);
    const auto wid = _document.model.data().make<wall>(aid, w.width);
    _document.model.data().get(wid).axis_offset = w.axis_offset;

    return {};
}

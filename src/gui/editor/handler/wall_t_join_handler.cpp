#include "wall_t_join_handler.h"

#include <iostream>
#include <cmath>

#include "views_take_model_variants.h"
#include "members_iterator.h"

using namespace gui::editor::handler;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;
using namespace corecad::model;
using namespace corecad::model::constraint;

wall_t_join_handler::wall_t_join_handler(doc::document &doc, floor_view &v, calc_tools& ct)
    : _document { doc }
    , _view { v }
    , _wall_snap_processor { doc, ct }
    , _ct { ct }
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

    const auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!ahid)
    {
        return false;
    }

    const auto tol = _view.model_interaction_tolerance();
    const double tol2 = tol.x * tol.y;

    for (const auto& p : _document.model.data().items<wall>())
    {
        const auto& axis = _document.model.data().get(p.second.axis);

        if (axis.s == ahid || axis.e == ahid)
        {
            continue;
        }

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

        // check if wall already has h/v alignment
        std::optional<corecad::model::coordinate2d> t_joint_wall_alignment;

        auto distances_pairs = _document.model.data().items<parameter::parameter>()
            | views::take_model_variants_with_ids<parameter::distance>();
        for (const auto& dp : distances_pairs)
        {
            const auto& d = dp.second;

            if (d.value == 0.0)
            {
                auto *wa_from = std::get_if<wall_axis_point::index_t>(&(d.from.val()));
                auto *wa_to = std::get_if<wall_axis_point::index_t>(&(d.to.val()));

                if (wa_from && wa_to 
                    && (*wa_from == sp.index || *wa_from == ep.index) 
                    && (*wa_to == sp.index || *wa_to == ep.index)
                )
                {
                    t_joint_wall_alignment = d.direction;
                    _t_joint_wall_alignment_id = dp.first;
                    break;
                }
            }
        }

        if (t_joint_wall_alignment)
        {
            _document.active_wall_snaps.add(
                parameter::parameter::create<parameter::distance>(
                    sp.index,
                    ahid,
                    0.0,
                    t_joint_wall_alignment.value()
                ),
                0.0,
                sp.index, ep.index
            );
            _document.active_wall_snaps.add(
                parameter::parameter::create<parameter::distance>(
                    ahid,
                    ep.index,
                    0.0,
                    t_joint_wall_alignment.value()
                ),
                0.0,
                sp.index, ep.index
            );
        }
        else
        {
            _document.active_wall_snaps.add(
                parameter::parameter::create<parameter::colinear>(
                    sp.index,
                    ahid,
                    ep.index
                ),
                0.0,
                sp.index, ep.index
            );
        }

        _wall_snap_processor.process();

        _t_joint_wall = p.first;

        const auto& ap = _document.model.data().get(ahid);
        model_pos.x = ap.x;
        model_pos.y = ap.y;

        return true;
    }

    _t_joint_wall = std::nullopt;
    return false;
}

post_apply_actions wall_t_join_handler::apply()
{
    if (!_t_joint_wall || !_document.active_handle)
    {
        return {};
    }

    const auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!ahid)
    {
        return {};
    }

    // split wall into 2 walls
    const auto& w = _document.model.data().get(_t_joint_wall.value());
    auto& a = _document.model.data().get(w.axis);

    const auto epid = a.e.val();
    a.e = ahid;

    const auto new_aid = _document.model.data().make<wall_axis_line>(ahid, epid);
    const auto new_wid = _document.model.data().make<wall>(new_aid, w.thickness);
    _document.model.data().get(new_wid).axis_offset = w.axis_offset;

    if (_t_joint_wall_alignment_id)
    {
        _document.model.data().erase(_t_joint_wall_alignment_id);
    }

    return {};
}

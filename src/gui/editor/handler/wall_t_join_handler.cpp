#include "wall_t_join_handler.h"

#include <iostream>
#include <cmath>

#include "views_take_model_variants.h"

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
    if (!_document.active_handle || _document.active_handle->handle_locators().empty())
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

    for (auto& p : _document.model.data().items<wall>())
    {
        auto is_wall_active = std::ranges::any_of(_document.active_handle->handle_locators(), [&p](const auto& hl) {
            if (const auto* wapl = std::get_if<parameter::wall_axis_point_locator>(&hl))
            {
                return wapl->wid == p.first;
            }

            return false;
        });

        if (is_wall_active)
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
        // TODO: review logic of creating aligned consraint.
        // current implementation has the issue with w/h alignments
        bool existing_snap = false;

        auto distances = _document.active_wall_snaps.parameters()
            | views::take_model_variants<parameter::distance>();
        for (const auto& d : distances)
        {
            if (d.value == 0.0)
            {
                auto from = _ct.point_resolver().resolve(d.from);
                auto *wa_from = std::get_if<wall_axis_point::index_t>(&from);
                if (wa_from && (*wa_from == sp.index || *wa_from == ep.index))
                {
                    existing_snap = true;
                    break;
                }
 
                auto to = _ct.point_resolver().resolve(d.to);
                auto *wa_to = std::get_if<wall_axis_point::index_t>(&to);
                if (wa_to && (*wa_to == sp.index || *wa_to == ep.index))
                {
                    existing_snap = true;
                    break;
                }
            }
        }

        if (!existing_snap)
        {
            _document.active_wall_snaps.add(
                parameter::parameter::create<parameter::colinear>(
                    parameter::wall_axis_point_locator { p.first, &wall_axis_line::s },
                    _document.active_handle.value().handle_locators()[0],
                    parameter::wall_axis_point_locator { p.first, &wall_axis_line::e }
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

std::optional<gui::doc::handle_data> wall_t_join_handler::apply()
{
    if (!_t_joint_wall || !_document.active_handle)
    {
        return std::nullopt;
    }

    const auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!ahid)
    {
        return std::nullopt;
    }

    // split wall into 2 walls
    const auto& w = _document.model.data().get(_t_joint_wall.value());
    auto& a = _document.model.data().get(w.axis);

    const auto epid = a.e.val();
    a.e = ahid;

    const auto aid = _document.model.data().make<wall_axis_line>(ahid, epid);
    const auto wid = _document.model.data().make<wall>(aid, w.width);
    _document.model.data().get(wid).axis_offset = w.axis_offset;

    return {};
}

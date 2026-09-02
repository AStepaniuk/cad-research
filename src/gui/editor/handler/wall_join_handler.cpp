#include "wall_join_handler.h"

#include <ranges>
#include <iostream>

#include "members_iterator.h"
#include "overloaded.h"
#include "property.h"

using namespace gui::editor::handler;
using namespace domain::plan::model::shape;
using namespace domain::plan::model::parameter;
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

    if (!_document.active_handle)
    {
        return false;
    } 

    const auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();

    for (const auto& p : _document.model.data().items<wall>())
    {
        const auto& a = _document.model.data().get(p.second.axis);

        auto check_point = [&](point_on_wall_axis_ptr pptr) {
            const auto& point_id = a.*pptr;
            if (point_id == ahid)
            {
                return false;
            }

            const auto& point = _document.model.data().get(a.*pptr);
        
            if (model_pos.x > point.x - tol.x && model_pos.x < point.x + tol.x
                && model_pos.y > point.y - tol.y && model_pos.y < point.y + tol.y)
            {
                _target_point_handle = doc::handle_data { point.index };

                model_pos.x = point.x;
                model_pos.y = point.y;

                return true;
            }
            return false;
        };

        if (check_point(&wall_axis_line::s))
        {
            return true;
        }
        if (check_point(&wall_axis_line::e))
        {
            return true;
        }
    }

    _target_point_handle = std::nullopt;

    return false;
}

template <typename TMember>
using point_id_property = is_property_of_type<TMember, wall_point_id_t>;

post_apply_actions wall_join_handler::apply()
{
    if (!_target_point_handle)
    {
        return {};
    }

    const auto tphid = _target_point_handle->handle_id_of_type<wall_axis_point>();
    if (!tphid)
    {
        return {};
    }

    const auto ahid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!ahid)
    {
        return {};
    }

    // update axises, which are connected to ahid, so they are connected to tphid instead
    for (auto&& [_, axis] : _document.model.data().items<wall_axis_line>())   
    {
        if (axis.s == ahid)
        {
            axis.s = tphid;
        }
        else if (axis.e == ahid)
        {
            axis.e = tphid;
        }
    }

    // update parameters ahid ==> tphid
    for (auto&& [_, p] : _document.model.data().items<parameter>())
    {
        std::visit([&](auto& param) {
                corecad::meta::visit_members<point_id_property>(param, [&](auto& prop) {
                    std::visit(corecad::util::overloaded {
                        [&] (wall_axis_point::index_t& a) {
                            if (a == ahid)
                            {
                                a = tphid;
                            }
                        },
                        [&] (wall_border_point::index_t& b) { }
                    }, prop.val());
                });
            }, p.instance
        );
    }

    return post_apply_actions { .new_active_handle = _target_point_handle };
}

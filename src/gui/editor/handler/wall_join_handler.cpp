#include "wall_join_handler.h"

#include <ranges>
#include <iostream>

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

    for (auto& p : _document.model.data().items<wall>())
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
                auto target_point_locators = _document.active_handle->handle_locators();
                target_point_locators.push_back(wall_axis_point_locator { p.first, pptr });

                _target_point_handle = doc::handle_data { target_point_locators, point.index };

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

post_apply_actions wall_join_handler::apply()
{
    if (!_target_point_handle || _target_point_handle->handle_locators().empty())
    {
        return {};
    }

    const auto tphid = _target_point_handle->handle_id_of_type<wall_axis_point>();
    if (!tphid)
    {
        return {};
    }

    for (const auto pl : _target_point_handle->handle_locators())
    {
        if (const auto* wapl = std::get_if<wall_axis_point_locator>(&pl))
        {
            auto& wall = _document.model.data().get(wapl->wid);
            auto& axis = _document.model.data().get(wall.axis);

            axis.*(wapl->point_on_axis_ptr) = tphid;
        }
    }

    return post_apply_actions { .new_active_handle = _target_point_handle };
}

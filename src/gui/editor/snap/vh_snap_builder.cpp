#include "vh_snap_builder.h"

using namespace gui::editor::snap;
using namespace corecad::model;
using namespace corecad::model::constraint;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;

vh_snap_builder::vh_snap_builder(doc::document &doc, floor_view &v)
    : _document { doc }
    , _floor_view { v }
{
}

void vh_snap_builder::calculate_snaps(float view_pos_x, float view_pos_y)
{
    if (!_document.active_handle)
    {
        return;
    }
    
    const auto apid = _document.active_handle->handle_id_of_type<wall_axis_point>();
    if (!apid)
    {
        return;
    }

    const auto model_pos = _floor_view.to_model(view_pos_x, view_pos_y);
    const auto tol = _floor_view.model_interaction_tolerance();

    const wall_axis_point* x_ref_point = nullptr;
    
    const wall_axis_point* y_ref_point = nullptr;

    double dx = 0.0;
    double dy = 0.0;

    for (const auto& pair : _document.model.data().items<wall>())
    {
        const auto& w = pair.second;

        auto process_point = [&](const wall& w, point_on_wall_axis_ptr powa, wall_axis_point::index_t spid) {
            if (apid == spid)
            {
                return;
            }

            const auto& p = _document.model.data().get(spid);

            if (model_pos.x > p.x - tol.x && model_pos.x < p.x + tol.x)
            {
                if (!x_ref_point || (std::abs(model_pos.y - p.y) < dy))
                {
                    dy = std::abs(model_pos.y - p.y);
                    x_ref_point = &p;
                }
            }

            if (model_pos.y > p.y - tol.y && model_pos.y < p.y + tol.y)
            {
                if (!y_ref_point || (std::abs(model_pos.x - p.x) < dx))
                {
                    dx = std::abs(model_pos.x - p.x);
                    y_ref_point = &p;
                }
            }
        };

        const auto& a = _document.model.data().get(w.axis);
        process_point(w, &wall_axis_line::s, a.s);
        process_point(w, &wall_axis_line::e, a.e);
    }

    if (x_ref_point)
    {
        auto parameter = parameter::parameter::create<parameter::distance>(
            x_ref_point->index
            , apid
            , 0.0
            , coordinate2d::x
        );

        const auto rank = std::abs(model_pos.x - x_ref_point->x);
        _document.active_wall_snaps.add(std::move(parameter), rank, x_ref_point->index);
    }

    if (y_ref_point)
    {
        auto parameter = parameter::parameter::create<parameter::distance>(
            y_ref_point->index
            , apid
            , 0.0
            , coordinate2d::y
        );

        const auto rank = std::abs(model_pos.y - y_ref_point->y);
        _document.active_wall_snaps.add(std::move(parameter), rank, y_ref_point->index);
    }
}

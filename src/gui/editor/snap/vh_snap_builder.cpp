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

    length_mm_t dx = 0.0 * mm;
    length_mm_t dy = 0.0 * mm;

    for (const auto& pair : _document.model.data().items<wall>())
    {
        const auto& w = pair.second;

        auto process_point = [&](const wall& w, point_on_wall_axis_ptr powa, wall_axis_point::index_t spid) {
            if (apid == spid)
            {
                return;
            }

            const auto& p = _document.model.data().get(spid);

            if (model_pos.x.val() > p.x.val() - tol.x.val() && model_pos.x.val() < p.x.val() + tol.x.val())
            {
                auto pdy = mp_units::abs(model_pos.y.val() - p.y.val());
                if (!x_ref_point || (pdy < dy))
                {
                    dy = pdy;
                    x_ref_point = &p;
                }
            }

            if (model_pos.y.val() > p.y.val() - tol.y.val() && model_pos.y.val() < p.y.val() + tol.y.val())
            {
                auto pdx = mp_units::abs(model_pos.x.val() - p.x.val());
                if (!y_ref_point || (pdx < dx))
                {
                    dx = pdx;
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
            , 0.0 * mm
            , coordinate2d::x
        );

        const auto rank = mp_units::abs(model_pos.x.val() - x_ref_point->x.val());
        _document.active_wall_snaps.add(std::move(parameter), rank.numerical_value_in(mm), x_ref_point->index);
    }

    if (y_ref_point)
    {
        auto parameter = parameter::parameter::create<parameter::distance>(
            y_ref_point->index
            , apid
            , 0.0 * mm
            , coordinate2d::y
        );

        const auto rank = mp_units::abs(model_pos.y.val() - y_ref_point->y.val());
        _document.active_wall_snaps.add(std::move(parameter), rank.numerical_value_in(mm), y_ref_point->index);
    }
}

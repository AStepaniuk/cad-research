#include "vh_snap_builder.h"

using namespace gui::editor::snap;
using namespace corecad::model;
using namespace corecad::model::constraint;
using namespace domain::plan::model;

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

    const auto model_pos = _floor_view.to_model(view_pos_x, view_pos_y);
    const auto tol = _floor_view.model_interaction_tolerance();

    // for (const auto spid : _document.selected_handles)
    for (const auto pair : _document.model.data().items<wall_axis_point>())
    {
        const auto spid = pair.first;
        if (_document.active_handle && _document.active_handle.value() == spid)
        {
            continue;
        } 

        const auto& p = _document.model.data().get(spid);

        if (model_pos.x > p.x - tol.x && model_pos.x < p.x + tol.x)
        {
            auto constraint = floor::constraint_t::create<floor::offset_wall_axis_point_t>(
                spid, _document.active_handle.value(),
                0.0, offset_direction::horizontal
            );

            const auto rank = std::abs(model_pos.x - p.x);
            _document.active_wall_snaps.add(std::move(constraint), rank, spid);
        }
        
        if (model_pos.y > p.y - tol.y && model_pos.y < p.y + tol.y)
        {
            auto constraint = floor::constraint_t::create<floor::offset_wall_axis_point_t>(
                spid, _document.active_handle.value(),
                0.0, offset_direction::vertical
            );

            const auto rank = std::abs(model_pos.y - p.y);
            _document.active_wall_snaps.add(std::move(constraint), rank, spid);
        }
    }   
}

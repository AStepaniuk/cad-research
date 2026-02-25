#include "wall_join_handler.h"

#include <ranges>

using namespace gui::editor::handler;
using namespace domain::plan::model;
using namespace corecad::model;

wall_join_handler::wall_join_handler(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
{
}

bool wall_join_handler::wall_move(
    float view_pos_x, float view_pos_y,
    vector2d& model_pos
)
{
    const auto tol = _view.model_interaction_tolerance();

    for (auto& p : _document.model.walls())
    {
        if (std::ranges::any_of(_document.active_walls, [&p](const auto& w_index) { return w_index == p.second.index; }))
        {
            continue;
        }

        const auto& sp = _document.model.points().get(p.second.start);
    
        if (model_pos.x > sp.x - tol.x && model_pos.x < sp.x + tol.x
        && model_pos.y > sp.y - tol.y && model_pos.y < sp.y + tol.y)
        {
            model_pos.x = sp.x;
            model_pos.y = sp.y;

            return true;
        }

        const auto& ep = _document.model.points().get(p.second.end);
    
        if (model_pos.x > ep.x - tol.x && model_pos.x < ep.x + tol.x
        && model_pos.y > ep.y - tol.y && model_pos.y < ep.y + tol.y)
        {
            model_pos.x = ep.x;
            model_pos.y = ep.y;

            return true;
        }
    }

    return false;
}

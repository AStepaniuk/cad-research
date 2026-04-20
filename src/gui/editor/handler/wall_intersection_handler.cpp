#include "wall_intersection_handler.h"

using namespace gui::editor::handler;
using namespace domain::plan::model;
using namespace corecad::model;

wall_intersection_handler::wall_intersection_handler(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
{
}

bool wall_intersection_handler::wall_move(
    float view_pos_x, float view_pos_y,
    wall_axis_point& model_pos
)
{
    return false;
}

void wall_intersection_handler::apply()
{
}

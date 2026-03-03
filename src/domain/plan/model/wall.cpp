#include "wall.h"

using namespace domain::plan::model;
using namespace corecad::model;

wall::wall(wall_axis_point::index_t s, wall_axis_point::index_t e, double w)
    : start { s }
    , end { e }
    , width { w }
{
}

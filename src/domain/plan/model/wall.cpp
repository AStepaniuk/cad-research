#include "wall.h"

using namespace domain::plan::model;
using namespace corecad::model;

wall::wall(vector2d::index_t s, vector2d::index_t e, double w)
    : start { s }
    , end { e }
    , width { w }
{
}

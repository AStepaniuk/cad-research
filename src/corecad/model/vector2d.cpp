#include "vector2d.h"

using namespace corecad::model;

vector2d::vector2d(double x, double y)
    : x { x }
    , y { y }
{
}

vector2d corecad::model::operator+(const vector2d &lhs, const vector2d &rhs)
{
    return vector2d { lhs.x + rhs.x, lhs.y + rhs.y };
}

vector2d corecad::model::operator-(const vector2d &lhs, const vector2d &rhs)
{
    return vector2d { lhs.x - rhs.x, lhs.y - rhs.y };
}

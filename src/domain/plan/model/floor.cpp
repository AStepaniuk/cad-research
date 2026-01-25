#include "floor.h"

using namespace domain::plan::model;

const floor::points2d_t& floor::points() const
{
    return _points;
}

floor::points2d_t& floor::points()
{
    return _points;
}

const floor::walls_t& floor::walls() const
{
    return _walls;
}

floor::walls_t& floor::walls()
{
    return _walls;
}

const floor::parameters_t &domain::plan::model::floor::parameters() const
{
    return _parameters;
}

floor::parameters_t &domain::plan::model::floor::parameters()
{
    return _parameters;
}

#include "floor.h"

using namespace domain::plan::model;

const floor::wall_axis_points_t& floor::wall_axis_points() const
{
    return _wall_axis_points;
}

floor::wall_axis_points_t& floor::wall_axis_points()
{
    return _wall_axis_points;
}

const floor::wall_border_points_t& floor::wall_border_points() const
{
    return _wall_border_points;
}

floor::wall_border_points_t& floor::wall_border_points()
{
    return _wall_border_points;
}

const floor::data_t& floor::data() const
{
    return _data;
}

floor::data_t& floor::data()
{
    return _data;
}

const floor::constraints_t &domain::plan::model::floor::parameters() const
{
    return _parameters;
}

floor::constraints_t &domain::plan::model::floor::parameters()
{
    return _parameters;
}

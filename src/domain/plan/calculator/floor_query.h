#pragma once

#include <vector>

#include "floor.h"
#include "point_locator.h"

namespace domain::plan::calculator
{
    class floor_query
    {
    public:
        floor_query(model::floor& floor);

        std::vector<model::parameter::point_locator_t> find_locators(model::shape::wall_axis_point::index_t pid);

        bool are_points_constrained_on_coordinate(
            const model::floor::constraint_t::point_id_t& p1,
            const model::floor::constraint_t::point_id_t& p2,
            corecad::model::coordinate2d coord    
        );
    
    private:
        model::floor &_floor;
    };
}

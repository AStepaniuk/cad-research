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


    
    private:
        model::floor &_floor;
    };
}

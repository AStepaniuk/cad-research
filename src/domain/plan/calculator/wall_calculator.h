#pragma once

#include "floor.h"

namespace domain { namespace plan { namespace calculator
{
    class wall_calculator
    {
        model::floor& _floor;

    public:
        wall_calculator(model::floor& floor);

        void recalculate_all_walls();
    };
}}}

#pragma once

#include "floor.h"

namespace domain { namespace plan { namespace calculator
{
    class wall_calculator
    {
    public:
        void recalculate_all_walls(model::floor& floor);
    };
}}}

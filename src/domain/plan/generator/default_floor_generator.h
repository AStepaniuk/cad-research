#pragma once

#include "floor.h"

namespace domain::plan::generator
{
    class default_floor_generator
    {
    public:
        void generate_floor(model::floor& floor);
    };
}

#pragma once

#include "floor.h"
#include "point_resolver.h"

namespace domain::plan::calculator
{
    class constraints_builder
    {
        model::floor& _floor;
        resolver::point_resolver& _pr;

    public:
        constraints_builder(model::floor& floor, resolver::point_resolver& pr);

        void rebuild_all_constraints();
    };
}

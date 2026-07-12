#pragma once

#include "floor.h"
#include "registry.h"
#include "point_resolver.h"

namespace domain::plan::calculator
{
    class constraints_builder
    {
        model::floor& _floor;
        resolver::point_resolver& _pr;

    public:
        constraints_builder(model::floor &floor, resolver::point_resolver& pr);

        void rebuild_all_constraints();
        void to_constraints(
            const corecad::model::registry<model::parameter::parameter>& from,
            corecad::model::registry<model::floor::constraint_t>& to
        ) const;

    private:
        model::floor::constraint_t to_constraint(const model::parameter::parameter& p) const;
    };
}

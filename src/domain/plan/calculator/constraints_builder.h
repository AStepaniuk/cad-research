#pragma once

#include <optional>

#include "floor.h"
#include "registry.h"
#include "floor_query.h"

namespace domain::plan::calculator
{
    class constraints_builder
    {
        model::floor& _floor;
        floor_query& _fq;

    public:
        constraints_builder(model::floor &floor, floor_query& fq);

        void rebuild_all_constraints();
        void to_constraints(
            const corecad::model::registry<model::parameter::parameter>& from,
            corecad::model::registry<model::floor::constraint_t>& to
        ) const;

    private:
        model::floor::constraint_t to_constraint(const model::parameter::parameter& p) const;

        enum class sign { pos, neg };
        struct wall_orientation
        {
            corecad::model::coordinate2d axis;
            sign s;

        };
        std::optional<wall_orientation> get_wall_orientation_axis(const model::shape::wall& w) const;
    };
}

#pragma once

#include <optional>

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

        void generate_vertical_wall_border_constraints(const model::shape::wall& w, const model::shape::wall_axis_line& a) const;
        void generate_horizontal_wall_border_constraints(const model::shape::wall& w, const model::shape::wall_axis_line& a) const;
        void generate_diagonal_wall_border_constraints(const model::shape::wall& w, const model::shape::wall_axis_line& a) const;

        enum class sign { pos, neg };
        struct wall_orientation
        {
            corecad::model::coordinate2d axis;
            sign s;

        };
        std::optional<wall_orientation> get_wall_orientation_axis(const model::shape::wall& w) const;
    };
}

#pragma once

#include "registry.h"

#include "constraint.h"
#include "fixed.h"
#include "offset.h"
#include "wall.h"

namespace domain { namespace plan { namespace model
{
    class floor
    {
    public:
        using constraint_t = corecad::model::constraint::constraint<
            corecad::model::constraint::fixed<wall_axis_point>,
            corecad::model::constraint::offset<wall_axis_point>
        >;

        using constraints_t = corecad::model::registry<constraint_t>;

        using wall_axis_points_t = corecad::model::registry<wall_axis_point>;
        using wall_border_points_t = corecad::model::registry<wall_border_point>;
        using walls_t = corecad::model::registry<wall>;

        const constraints_t& parameters() const;
        constraints_t& parameters();

        const wall_axis_points_t& wall_axis_points() const;
        wall_axis_points_t& wall_axis_points();

        const wall_border_points_t& wall_border_points() const;
        wall_border_points_t& wall_border_points();

        const walls_t& walls() const;
        walls_t& walls();
    
    private:
        constraints_t _parameters;

        wall_axis_points_t _wall_axis_points;
        wall_border_points_t _wall_border_points;
        walls_t _walls;
    };
}}}

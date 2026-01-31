#pragma once

#include "registry.h"

#include "constraint.h"
#include "vector2d.h"
#include "wall.h"

namespace domain { namespace plan { namespace model
{
    class floor
    {
    public:
        using parameters_t = corecad::model::registry<corecad::model::constraint::constraint>;
        using points2d_t = corecad::model::registry<corecad::model::vector2d>;
        using walls_t = corecad::model::registry<wall>;

        const parameters_t& parameters() const;
        parameters_t& parameters();

        const points2d_t& points() const;
        points2d_t& points();

        const walls_t& walls() const;
        walls_t& walls();
    
    private:
        parameters_t _parameters;

        points2d_t _points;
        walls_t _walls;
    };
}}}

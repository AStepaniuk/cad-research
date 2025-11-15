#pragma once

#include "registry.h"

#include "vector2d.h"
#include "wall.h"

namespace domain { namespace plan { namespace model
{
    class floor
    {
    public:
        using points2d_t = corecad::model::registry<corecad::model::vector2d>;
        using walls_t = corecad::model::registry<wall>;

        const points2d_t& points() const;
        points2d_t& points();

        const walls_t& walls() const;
        walls_t& walls();
    
    private:
        points2d_t _points;
        walls_t _walls;
    };
}}}

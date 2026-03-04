#pragma once

#include "registry_pool.h"

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

        using data_t = corecad::model::registry_pool<
            constraint_t,
            wall_axis_point,
            wall_border_point,
            wall
        >;

        const data_t& data() const;
        data_t& data();
    
    private:
        data_t _data;

    public:
        using constraints_t = std::remove_cvref_t<decltype(_data.items<constraint_t>())>;        
    };
}}}

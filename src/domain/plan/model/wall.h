#pragma once

#include "model_base.h"
#include "vector2d.h" 

namespace domain { namespace plan { namespace model
{
    class wall_axis_tag {};
    class wall_border_tag {};

    using wall_axis_point = corecad::model::vector2d<wall_axis_tag>;
    using wall_border_point = corecad::model::vector2d<wall_border_tag>;

    class wall : public corecad::model::model_base<wall>
    {
    public:
        wall(wall_axis_point::index_t b, wall_axis_point::index_t e, double w);

        // primary model properties
        wall_axis_point::index_t start;
        wall_axis_point::index_t end;

        double width;

        // calculated properties
        wall_border_point::index_t start_left;
        wall_border_point::index_t start_right;
        wall_border_point::index_t end_left;
        wall_border_point::index_t end_right;

        unsigned int start_joints;
        unsigned int end_joints;
    };  
}}}

#pragma once

#include "model_base.h"
#include "vector2d.h"
#include "property.h"

namespace domain { namespace plan { namespace model
{
    class wall_axis_tag {};
    class wall_border_tag {};

    using wall_axis_point = corecad::model::vector2d<wall_axis_tag>;
    using wall_border_point = corecad::model::vector2d<wall_border_tag>;

    class wall : public corecad::model::model_base<wall>
    {
    public:
        wall(wall_axis_point::index_t s, wall_axis_point::index_t e, double w)
            : start { this, s }
            , end { this, e }
            , width { this, w }
        {
        }

        wall(const wall& other)
            : corecad::model::model_base<wall> { other }
            , start { this, other.start }
            , end { this, other.end }
            , width { this, other.width }
            , start_left { other.start_left }
            , start_right { other.start_right }
            , end_left { other.end_left }
            , end_right { other.end_right }
            , start_joints { other.start_joints }
            , end_joints { other.end_joints }
        {
        }

        wall(wall&& other)
            : corecad::model::model_base<wall> { other }
            , start { this, other.start }
            , end { this, other.end }
            , width { this, other.width }
            , start_left { other.start_left }
            , start_right { other.start_right }
            , end_left { other.end_left }
            , end_right { other.end_right }
            , start_joints { other.start_joints }
            , end_joints { other.end_joints }
        {
        }

        wall& operator=(const wall& other) = default;
        wall& operator=(wall&& other) = default;

        // primary model properties
        corecad::model::property<wall_axis_point::index_t, wall> start;
        corecad::model::property<wall_axis_point::index_t, wall> end;

        corecad::model::property<double, wall> width;

        // calculated properties
        wall_border_point::index_t start_left;
        wall_border_point::index_t start_right;
        wall_border_point::index_t end_left;
        wall_border_point::index_t end_right;

        unsigned int start_joints;
        unsigned int end_joints;
    };  
}}}

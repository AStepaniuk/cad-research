#pragma once

#include "model_base.h"
#include "registry.h"
#include "vector2d.h"
#include "line2d.h"
#include "property.h"

namespace domain { namespace plan { namespace model
{
    class wall_axis_tag {};
    class wall_border_tag {};

    using wall_axis_point = corecad::model::vector2d<wall_axis_tag>;
    using wall_axis_line = corecad::model::line2d<wall_axis_tag>;

    using wall_border_point = corecad::model::vector2d<wall_border_tag>;

    class wall : public corecad::model::model_base<wall>
    {
    public:
        wall(wall_axis_line::index_t a, double w)
            : axis { this, a }
            , width { this, w }
            , axis_offset { this, 0.0 }
        {
        }

        wall(const wall& other)
            : corecad::model::model_base<wall> { other }
            , axis { this, other.axis }
            , width { this, other.width }
            , axis_offset { this, other.axis_offset }
            , start_left { other.start_left }
            , start_right { other.start_right }
            , end_left { other.end_left }
            , end_right { other.end_right }
        {
        }

        wall(wall&& other)
            : corecad::model::model_base<wall> { other }
            , axis { this, other.axis }
            , width { this, other.width }
            , axis_offset { this, other.axis_offset }
            , start_left { other.start_left }
            , start_right { other.start_right }
            , end_left { other.end_left }
            , end_right { other.end_right }
        {
        }

        wall& operator=(const wall& other) = default;
        wall& operator=(wall&& other) = default;

        void reset_properties_updated()
        {
            axis.reset_updated();
            width.reset_updated();
        }

        // primary model properties
        corecad::model::property<wall_axis_line::index_t, wall> axis;

        corecad::model::property<double, wall> width;
        corecad::model::property<double, wall> axis_offset;

        // calculated properties
        wall_border_point::index_t start_left;
        wall_border_point::index_t start_right;
        wall_border_point::index_t end_left;
        wall_border_point::index_t end_right;
    };  

    std::ostream& operator<<(std::ostream& os, const wall& w);
}}}

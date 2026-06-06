#pragma once

#include "model_base.h"
#include "registry.h"
#include "vector2d.h"
#include "line2d.h"
#include "property.h"

namespace domain::plan::model::shape
{
    class wall_axis_tag {};
    class wall_border_tag {};

    using wall_axis_point = corecad::model::vector2d<wall_axis_tag>;
    using wall_axis_line = corecad::model::line2d<wall_axis_tag>;

    using wall_border_point = corecad::model::vector2d<wall_border_tag>;
    using wall_border_line = corecad::model::line2d<wall_border_tag>;
    using point_on_wall_border_ptr = corecad::model::point_on_line_ptr<wall_border_tag>;

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
            , left { other.left }
            , right { other.right }
            , start_stub { other.start_stub }
            , end_stub { other.end_stub }
        {
        }

        wall(wall&& other)
            : corecad::model::model_base<wall> { other }
            , axis { this, other.axis }
            , width { this, other.width }
            , axis_offset { this, other.axis_offset }
            , left { other.left }
            , right { other.right }
            , start_stub { other.start_stub }
            , end_stub { other.end_stub }
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
        wall_border_line::index_t left {};
        wall_border_line::index_t right {};
        wall_border_line::index_t start_stub {};
        wall_border_line::index_t end_stub {};
    };  

    std::ostream& operator<<(std::ostream& os, const wall& w);

    using wall_border_line_ptr = wall_border_line::index_t wall::*;
}

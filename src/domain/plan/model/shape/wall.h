#pragma once

#include "model_base.h"
#include "registry.h"
#include "type_list.h"
#include "vector2d.h"
#include "line2d.h"
#include "property.h"

namespace domain::plan::model::shape
{
    class wall_axis_tag {};
    class wall_border_tag {};

    using wall_axis_point = corecad::model::vector2d<wall_axis_tag>;
    using wall_axis_line = corecad::model::line2d<wall_axis_tag>;
    using point_on_wall_axis_ptr = corecad::model::point_on_line_ptr<wall_axis_tag>;

    using wall_border_point = corecad::model::vector2d<wall_border_tag>;
    using wall_border_line = corecad::model::line2d<wall_border_tag>;
    using point_on_wall_border_ptr = corecad::model::point_on_line_ptr<wall_border_tag>;

    using wall_points_tl = corecad::util::type_list<wall_axis_point, wall_border_point>;
    using wall_points_ids_tl = corecad::model::to_index_type_list<wall_points_tl>::type;

    std::ostream& operator<<(std::ostream& os, const wall_points_tl::variant_t& val);
    std::ostream& operator<<(std::ostream& os, const wall_points_ids_tl::variant_t& val);

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

        wall(wall&& other) noexcept
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
        wall& operator=(wall&& other) noexcept = default;

        // primary model properties
        corecad::model::property<wall_axis_line::index_t, wall> axis;

        corecad::model::property<double, wall> width;
        corecad::model::property<double, wall> axis_offset;

        // calculated properties
        wall_border_line::index_t left {};
        wall_border_line::index_t right {};
        wall_border_line::index_t start_stub {};
        wall_border_line::index_t end_stub {};

        static constexpr auto members_metadata = std::make_tuple(
            &wall::index,
            &wall::width,
            &wall::axis_offset,
            &wall::left,
            &wall::right,
            &wall::start_stub,
            &wall::end_stub
        );
    };  

    std::ostream& operator<<(std::ostream& os, const wall& w);

    using wall_border_line_ptr = wall_border_line::index_t wall::*;
}

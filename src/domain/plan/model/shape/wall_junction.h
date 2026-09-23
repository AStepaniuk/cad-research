#pragma once

#include "wall.h"

namespace domain::plan::model::shape
{
    enum class junction_join_style
    {
        automatic,  // Default: Geometry is derived dynamically from layer priorities and materials
        miter,      // Override: Forces a 45-degree geometric cut across all layers
        butt,       // Override: Forces one wall to act as a flat stopping plane
        disallowed  // Override: Bypasses the geometry engine; walls remain flat-capped independent shapes
    };

    class wall_junction : public corecad::model::model_base<wall_junction>
    {
    public:
        template <typename TValue> using property = corecad::model::property<TValue, wall_junction>;
        template <typename TValue> using list = corecad::model::list<TValue, wall_junction>;

        property<wall_axis_point::index_t> axis_point;
        
        // The explicit override layout configuration chosen by the user.
        property<junction_join_style> join_style;

        wall_junction()
            : axis_point { this, {} }
            , join_style { this, junction_join_style::automatic }
        {}

        wall_junction(const wall_junction& other)
            : corecad::model::model_base<wall_junction> { other }
            , axis_point { this, other.axis_point }
            , join_style { this, other.join_style }
        {}

        wall_junction(wall_junction&& other) noexcept
            : corecad::model::model_base<wall_junction> { std::move(other) }
            , axis_point { this, std::move(other.axis_point) }
            , join_style { this, std::move(other.join_style) }
        {}

        wall_junction& operator=(const wall_junction&) = default;
        wall_junction& operator=(wall_junction&&) noexcept = default;

        struct metadata
        {
            static constexpr std::string_view type_name = "wall_junction"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("index", &wall_junction::index),
                corecad::meta::member("axis_point", &wall_junction::axis_point),
                corecad::meta::member("join_style", &wall_junction::join_style)
            );
        };
    };
}

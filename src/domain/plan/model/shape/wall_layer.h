#pragma once

#include <string>

#include "model_base.h"
#include "wall_material_definition.h"

namespace domain::plan::model::shape
{
    enum class wall_layer_function
    {
        load_bearing,
        substrate,
        insulation,
        outer_finish,
        inner_finish
    };

    class wall_layer : public corecad::model::model_base<wall_layer>
    {
    public:
        template <typename TValue>
        using property = corecad::model::property<TValue, wall_layer>;
    
        wall_layer()
            : material { this, {} }
            , thickness { this, 0.0 }
            , function { this, wall_layer_function::load_bearing }
            , priority { this, 1000 }
            , wraps_at_ends { this, false }
            , wraps_at_inserts { this, false }
        {}

        wall_layer(const wall_layer& other)
            : corecad::model::model_base<wall_layer> { other }
            , material { this, other.material }
            , thickness { this, other.thickness }
            , function { this, other.function }
            , priority { this, other.priority }
            , wraps_at_ends { this, other.wraps_at_ends }
            , wraps_at_inserts { this, other.wraps_at_inserts }
        {}

        wall_layer(wall_layer&& other) noexcept
            : corecad::model::model_base<wall_layer> { other }
            , material { this, other.material }
            , thickness { this, other.thickness }
            , function { this, other.function }
            , priority { this, other.priority }
            , wraps_at_ends { this, other.wraps_at_ends }
            , wraps_at_inserts { this, other.wraps_at_inserts }
        {}

        wall_layer& operator=(const wall_layer&) = default;
        wall_layer& operator=(wall_layer&&) = default;

        property<wall_material_definition::index_t> material;
        property<double> thickness;
        property<wall_layer_function> function;

        // Priority dictates intersection rules. 
        // Scale 0 to 1000. Higher numbers cut cleanly through lower numbers.
        // Structural core = 1000, Substrate = 600, Insulation = 400, Finishes = 100
        property<int> priority; 

        // BIM Wrapping flags: dictates if this finish layer wraps around 
        // exposed wall stubs, window inserts, or T-joint edges.
        property<bool> wraps_at_ends;
        property<bool> wraps_at_inserts;

        struct metadata
        {
            static constexpr std::string_view type_name = "wall_layer"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("index", &wall_layer::index),
                corecad::meta::member("material", &wall_layer::material),
                corecad::meta::member("thickness", &wall_layer::thickness),
                corecad::meta::member("function", &wall_layer::function),
                corecad::meta::member("priority", &wall_layer::priority),
                corecad::meta::member("wraps_at_ends", &wall_layer::wraps_at_ends),
                corecad::meta::member("wraps_at_inserts", &wall_layer::wraps_at_inserts)
            );
        };
    };
}

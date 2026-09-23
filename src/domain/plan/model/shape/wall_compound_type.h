#pragma once

#include <string>

#include "model_base.h"
#include "wall_layer.h"

namespace domain::plan::model::shape
{
    enum class wall_structural_role
    {
        shear_wall = 1000,
        bearing_exterior = 800,
        bearing_interior = 600,
        partition_wall = 200
    };
    
    class wall_compound_type : public corecad::model::model_base<wall_compound_type>
    {
    public:
        template <typename TValue>
        using property = corecad::model::property<TValue, wall_compound_type>;

        template <typename TValue>
        using list = corecad::model::list<TValue, wall_compound_type>;

        property<std::string> name;
        list<wall_layer::index_t> layers;
        property<wall_structural_role> structural_role;

        wall_compound_type()
            : name { this, {} }
            , layers { this }
            , structural_role { this, wall_structural_role::partition_wall }
        {}

        wall_compound_type(const wall_compound_type& other)
            : corecad::model::model_base<wall_compound_type> { other }
            , name { this, other.name }
            , layers { this, other.layers }
            , structural_role { this, other.structural_role }
        {}

        wall_compound_type(wall_compound_type&& other)
            : corecad::model::model_base<wall_compound_type> { other }
            , name { this, std::move(other.name) }
            , layers { this, std::move(other.layers) }
            , structural_role { this, std::move(other.structural_role) }
        {}

        wall_compound_type& operator=(const wall_compound_type&) = default;
        wall_compound_type& operator=(wall_compound_type&&) noexcept = default;

        struct metadata
        {
            static constexpr std::string_view type_name = "wall_compound_type"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("index", &wall_compound_type::index),
                corecad::meta::member("name", &wall_compound_type::name),
                corecad::meta::member("layers", &wall_compound_type::layers),
                corecad::meta::member("structural_role", &wall_compound_type::structural_role)
            );
        };
    };
}
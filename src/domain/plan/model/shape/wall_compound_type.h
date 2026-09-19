#pragma once

#include <string>

#include "model_base.h"
#include "wall_layer.h"

namespace domain::plan::model::shape
{
    class wall_compound_type : public corecad::model::model_base<wall_compound_type>
    {
    public:
        template <typename TValue>
        using property = corecad::model::property<TValue, wall_compound_type>;

        template <typename TValue>
        using list = corecad::model::list<TValue, wall_compound_type>;

        property<std::string> name;
        list<wall_layer::index_t> layers;

        wall_compound_type()
            : name { this, {} }
            , layers { this }
        {}

        wall_compound_type(const wall_compound_type& other)
            : name { this, other.name }
            , layers { this, other.layers }
        {}

        wall_compound_type(wall_compound_type&& other)
            : name { this, std::move(other.name) }
            , layers { this, std::move(other.layers) }
        {}

        wall_compound_type& operator=(const wall_compound_type&) = default;
        wall_compound_type& operator=(wall_compound_type&&) noexcept = default;

        struct metadata
        {
            static constexpr std::string_view type_name = "wall_compound_type"; 

            static constexpr auto members = std::make_tuple(
                corecad::meta::member("index", &wall_compound_type::index),
                corecad::meta::member("name", &wall_compound_type::name),
                corecad::meta::member("layers", &wall_compound_type::layers)
            );
        };
    };
}
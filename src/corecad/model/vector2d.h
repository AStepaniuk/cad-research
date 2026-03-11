#pragma once

#include <type_traits>

#include "model_base.h"
#include "registry.h"
#include "property.h"

namespace corecad { namespace model
{
    template<typename Tag>
    class vector2d : public corecad::model::model_base<vector2d<Tag>>
    {
    public:
        vector2d(double x, double y)
            : x { this, x }
            , y { this, y }
        {
        }

        vector2d(const vector2d& other) 
            : corecad::model::model_base<vector2d<Tag>> { other }
            , x { this, static_cast<double>(other.x) }
            , y { this, static_cast<double>(other.y) } 
        {
        }

        vector2d(vector2d&& other) noexcept
            : corecad::model::model_base<vector2d<Tag>> { other }
            , x { this, static_cast<double>(other.x) }
            , y { this, static_cast<double>(other.y) }
        {
        }

        template<typename TagOther>
        explicit vector2d(const vector2d<TagOther>& other)
            : x { this, static_cast<double>(other.x) }
            , y { this, static_cast<double>(other.y) }
        {
        }

        vector2d& operator=(const vector2d& other) = default;
        vector2d& operator=(vector2d&& other) noexcept = default;

        void reset_properties_updated()
        {
            x.reset_updated();
            y.reset_updated();
        }

        property<double, vector2d> x { *this };
        property<double, vector2d> y { *this };
    };  

    template<typename Tag>
    vector2d<Tag> operator+(const vector2d<Tag>& lhs, const vector2d<Tag>& rhs)
    {
        return vector2d<Tag> { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template<typename Tag>
    vector2d<Tag> operator-(const vector2d<Tag>& lhs, const vector2d<Tag>& rhs)
    {
        return vector2d<Tag> { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    // concept to check if a type is a template instantiation of a vector2d

    template <typename T>
    struct is_vector2d : std::false_type {};

    template <typename Tag>
    struct is_vector2d<vector2d<Tag>> : std::true_type {};

    template <typename T>
    concept IsVector2D = is_vector2d<std::remove_cvref_t<T>>::value;
}}

#pragma once

#include <type_traits>

#include "model_base.h"

namespace corecad { namespace model
{
    template<typename Tag>
    class vector2d : public corecad::model::model_base<vector2d<Tag>>
    {
    public:
        vector2d(double x, double y)
            : x { x }
            , y { y }
        {
        }

        template<typename TagOther>
        explicit vector2d(const vector2d<TagOther>& other)
            : x { other.x }
            , y { other.y }
        {
        }

        double x;
        double y;
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

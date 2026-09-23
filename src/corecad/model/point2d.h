#pragma once

#include <type_traits>

#include "model_base.h"
#include "registry.h"
#include "type_meta_info.h"
#include "member_info.h"
#include "vector2d.h"
#include "units.h"

namespace corecad::model
{
    template<typename Tag>
    class point2d : public corecad::model::model_base<point2d<Tag>>
    {
    public:
        point2d(length_mm_t x, length_mm_t y)
            : x { this, x }
            , y { this, y }
        {
        }

        point2d(const point2d& other) 
            : corecad::model::model_base<point2d<Tag>> { other }
            , x { this, static_cast<length_mm_t>(other.x) }
            , y { this, static_cast<length_mm_t>(other.y) } 
        {
        }

        point2d(point2d&& other) noexcept
            : corecad::model::model_base<point2d<Tag>> { other }
            , x { this, static_cast<length_mm_t>(other.x) }
            , y { this, static_cast<length_mm_t>(other.y) }
        {
        }

        explicit point2d(const math::vector2d<length_mm_t>& v) noexcept
            : x { this, v.x }
            , y { this, v.y }
        {
        }

        template<typename TagOther>
        explicit point2d(const point2d<TagOther>& other)
            : x { this, static_cast<length_mm_t>(other.x) }
            , y { this, static_cast<length_mm_t>(other.y) }
        {
        }

        point2d& operator=(const point2d& other) = default;
        point2d& operator=(point2d&& other) noexcept = default;

        property<length_mm_t, point2d> x { *this };
        property<length_mm_t, point2d> y { *this };

        math::vector2d<length_mm_t> vector() const
        {
            return { x, y };
        }

        operator math::vector2d<length_mm_t>() const
        {
            return vector();
        }

        struct metadata
        {
            static constexpr std::string_view base_type_name = "point2d_";
            static constexpr std::string_view tag_name = corecad::meta::type_name<Tag>();
            static constexpr std::string_view type_name = corecad::util::join_v<base_type_name, tag_name>;

            static constexpr auto members = std::make_tuple(
                meta::member("index", &point2d::index),
                meta::member("x", &point2d::x),
                meta::member("y", &point2d::y)
            );
        };
    };  

    template<typename Tag>
    std::ostream& operator<<(std::ostream& os, const point2d<Tag>& v)
    {
        return os << static_cast<const corecad::model::model_base<point2d<Tag>>&>(v) << " x:" << v.x << " y:" << v.y;
    }

    // concept to check if a type is a template instantiation of a point2d

    template <typename T>
    struct is_point2d : std::false_type {};

    template <typename Tag>
    struct is_point2d<point2d<Tag>> : std::true_type {};

    template <typename T>
    concept IsPoint2D = is_point2d<std::remove_cvref_t<T>>::value;


    template <typename T>
    struct is_point2d_index : std::false_type {};

    template <typename Tag>
    struct is_point2d_index<registry_index_t<Tag>> : std::bool_constant<is_point2d<Tag>::value> {};

    template <typename T>
    concept IsPoint2DIndex = is_point2d_index<std::remove_cvref_t<T>>::value;
}

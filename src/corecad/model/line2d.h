#pragma once

#pragma once

#include <type_traits>

#include "model_base.h"
#include "vector2d.h"
#include "registry.h"
#include "property.h"

namespace corecad::model
{
    template<typename Tag>
    class line2d : public corecad::model::model_base<line2d<Tag>>
    {
    public:
        using point_t = vector2d<Tag>;
        using point_property_t = property<typename point_t::index_t, line2d>;
        using base_t = corecad::model::model_base<line2d>;

        line2d(point_t::index_t start, point_t::index_t end)
            : s { this, start }
            , e { this, end }
        {
        }

        line2d(const line2d& other) 
            : base_t { other }
            , s { this, other.s }
            , e { this, other.e } 
        {
        }

        line2d(line2d&& other) noexcept
            : base_t { std::move(other) }
            , s { this, other.s }
            , e { this, other.e }
        {
        }

        line2d& operator=(const line2d& other) = default;
        line2d& operator=(line2d&& other) noexcept = default;

        point_property_t s;
        point_property_t e;

        static constexpr auto members_metadata = std::make_tuple(
            &line2d::index,
            &line2d::s,
            &line2d::e
        );
    };  

    template<typename Tag>
    std::ostream& operator<<(std::ostream& os, const line2d<Tag>& l)
    {
        using base_t = corecad::model::model_base<line2d<Tag>>;
        return os << static_cast<const base_t&>(l) << ": " << l.s << " - " << l.e;
    }


    template <typename Tag>
    using point_on_line_ptr = typename line2d<Tag>::point_property_t line2d<Tag>::*;

    template <typename Tag>
    std::ostream& operator<<(std::ostream& os, const point_on_line_ptr<Tag>& pol)
    {
        if (pol == &line2d<Tag>::s) return os << 's';
        if (pol == &line2d<Tag>::e) return os << 'e';
        if (pol == nullptr) return os << "null";
        else return os << "??";
    }

    template <typename Tag>
    point_on_line_ptr<Tag> opposite(point_on_line_ptr<Tag> pol)
    {
        return pol == &line2d<Tag>::s ? &line2d<Tag>::e : &line2d<Tag>::s;
    };


    template <typename T>
    struct is_line2d : std::false_type {};

    template <typename Tag>
    struct is_line2d<line2d<Tag>> : std::true_type {};

    template <typename T>
    concept IsLine2D = is_line2d<std::remove_cvref_t<T>>::value;
}

#pragma once

#pragma once

#include <type_traits>

#include "model_base.h"
#include "vector2d.h"
#include "registry.h"
#include "property.h"

namespace corecad { namespace model
{
    template<typename Tag>
    class line2d : public corecad::model::model_base<line2d<Tag>>
    {
    public:
        using point_t = vector2d<Tag>;

        line2d(point_t::index_t start, point_t::index_t end)
            : s { this, start }
            , e { this, end }
        {
        }

        line2d(const line2d& other) 
            : corecad::model::model_base<line2d> { other }
            , s { this, other.s }
            , e { this, other.e } 
        {
        }

        line2d(line2d&& other) noexcept
            : corecad::model::model_base<line2d> { other }
            , s { this, other.s }
            , e { this, other.e }
        {
        }

        line2d& operator=(const line2d& other) = default;
        line2d& operator=(line2d&& other) noexcept = default;

        void reset_properties_updated()
        {
            s.reset_updated();
            e.reset_updated();
        }

        property<typename point_t::index_t, line2d> s;
        property<typename point_t::index_t, line2d> e;
    };  

    template<typename Tag>
    std::ostream& operator<<(std::ostream& os, const line2d<Tag>& l)
    {
        return os << static_cast<const corecad::model::model_base<line2d<Tag>>&>(l) << ": " << l.s << " - " << l.e;
    }

    template <typename T>
    struct is_line2d : std::false_type {};

    template <typename Tag>
    struct is_line2d<line2d<Tag>> : std::true_type {};

    template <typename T>
    concept IsLine2D = is_line2d<std::remove_cvref_t<T>>::value;
}}

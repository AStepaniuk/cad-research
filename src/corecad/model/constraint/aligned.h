#pragma once

#include "vector2d.h"
#include "property.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct aligned
    {
        using point_id_t = TVector2DIndexList::variant_t;

        aligned(point_id_t p1, point_id_t p2, point_id_t p3)
            : point1 { nullptr, p1 }
            , point2 { nullptr, p2 }
            , point3 { nullptr, p3 }
        {
        }

        property<point_id_t, TModel> point1;
        property<point_id_t, TModel> point2;
        property<point_id_t, TModel> point3;

        struct metadata
        {
            static constexpr std::string_view type_name = "aligned"; 

            static constexpr auto members = std::make_tuple(
                meta::member("point1", &aligned::point1),
                meta::member("point2", &aligned::point2),
                meta::member("point3", &aligned::point3)
            );
        };
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const aligned<TVector2DIndexList, TModel>& a)
    {
        return os << a.point1 << " - " << a.point2 << " - " << a.point3;
    }
}

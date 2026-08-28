#pragma once

#include "vector2d.h"
#include "coordinate2d.h"
#include "property.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct fixed
    {
        using point_id_t = TVector2DIndexList::variant_t;

        fixed(point_id_t p, double v, coordinate2d c)
            : coordinate { nullptr, c }
            , value { nullptr, v }
            , point { nullptr, p }
        {}

        property<coordinate2d, TModel> coordinate;
        property<double, TModel> value;

        property<point_id_t, TModel> point;

        struct metadata
        {
            static constexpr std::string_view type_name = "fixed"; 

            static constexpr auto members = std::make_tuple(
                meta::member("value", &fixed::value),
                meta::member("coordinate", &fixed::coordinate),
                meta::member("point", &fixed::point)
            );
        };
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const fixed<TVector2DIndexList, TModel>& f)
    {
        return os << f.coordinate << "=" << f.value << " p:" << f.point;
    }
}

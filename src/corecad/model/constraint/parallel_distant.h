#pragma once

#include "vector2d.h"
#include "property.h"
#include "type_list.h"

namespace corecad::model::constraint
{
    template <typename TVector2DIndexList, typename TModel>
    requires util::AllElementsAre<TVector2DIndexList, is_vector2d_index>
    struct parallel_distant
    {
        using point_id_t = TVector2DIndexList::variant_t;

        parallel_distant(point_id_t l1s, point_id_t l1e, point_id_t l2s, point_id_t l2e, double d)
            : line1_start { nullptr, l1s }
            , line1_end { nullptr, l1e }
            , line2_start { nullptr, l2s }
            , line2_end { nullptr, l2e }
            , distance { nullptr, d }
        {
        }

        property<point_id_t, TModel> line1_start;
        property<point_id_t, TModel> line1_end;
        property<point_id_t, TModel> line2_start;
        property<point_id_t, TModel> line2_end;
        property<double, TModel> distance;

        static constexpr auto members_metadata = std::make_tuple(
            &parallel_distant::line1_start,
            &parallel_distant::line1_end,
            &parallel_distant::line2_start,
            &parallel_distant::line2_end,
            &parallel_distant::distance
        );
    };

    template <typename TVector2DIndexList, typename TModel>
    std::ostream& operator<<(std::ostream& os, const parallel_distant<TVector2DIndexList, TModel>& pd)
    {
        return os << pd.line1_start << "-" << pd.line1_end
            << " // "
            << pd.line2_start << "-" << pd.line2_end
            << " d=" << pd.distance;
    }
}

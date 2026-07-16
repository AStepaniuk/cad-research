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

        void bind(TModel& parent)
        {
            line1_start.bind(parent);
            line1_end.bind(parent);
            line2_start.bind(parent);
            line2_end.bind(parent);
            distance.bind(parent);
        }

        void reset_properties_updated()
        {
            line1_start.reset_updated();
            line1_end.reset_updated();
            line2_start.reset_updated();
            line2_end.reset_updated();
            distance.reset_updated();
        }
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

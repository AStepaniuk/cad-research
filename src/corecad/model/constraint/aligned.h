#pragma once

#include "vector2d.h"
#include "property.h"

#include <vector>

namespace corecad { namespace model { namespace constraint
{
    template<IsVector2D TVector, typename TModel>
    struct aligned
    {
        using vector2d_prop_t = property<typename TVector::index_t, TModel>;

    public:
        aligned(TVector::index_t p1, TVector::index_t p2, TVector::index_t p3)
            : point1 { nullptr, p1 }
            , point2 { nullptr, p2 }
            , point3 { nullptr, p3 }
        {
        }

        property<typename TVector::index_t, TModel> point1;
        property<typename TVector::index_t, TModel> point2;
        property<typename TVector::index_t, TModel> point3;

        void bind(TModel& parent)
        {
            point1.bind(parent);
            point2.bind(parent);
            point3.bind(parent);
        }

        void reset_properties_updated()
        {
            point1.reset_updated();
            point2.reset_updated();
            point3.reset_updated();
        }
    };

    template<IsVector2D TVector, typename TModel>
    std::ostream& operator<<(std::ostream& os, const aligned<TVector, TModel>& a)
    {
        return os << a.point1 << " - " << a.point2 << " - " << a.point3;
    }
}}}
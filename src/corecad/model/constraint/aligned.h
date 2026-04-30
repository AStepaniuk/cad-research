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

        std::vector<vector2d_prop_t> _points;

    public:
        aligned(TVector::index_t p1, TVector::index_t p2, TVector::index_t p3)
        {
            _points.push_back(vector2d_prop_t { nullptr, p1 });
            _points.push_back(vector2d_prop_t { nullptr, p2 });
            _points.push_back(vector2d_prop_t { nullptr, p3 });
        }

        void bind(TModel& parent)
        {
            for (auto& p : _points)
            {
                p.bind(parent);
            }
        }

        void reset_properties_updated()
        {
            for (auto& p : _points)
            {
                p.reset_updated();
            }
        }

        const std::vector<vector2d_prop_t> points() const
        {
            return _points;
        }
    };

    template<IsVector2D TVector, typename TModel>
    std::ostream& operator<<(std::ostream& os, const aligned<TVector, TModel>& a)
    {
        bool first = true;

        for (const auto& p : a.points())
        {
            if (first)
            {
                os << " - ";
            }
            os << p.val();
        }

        return os;
    }
}}}
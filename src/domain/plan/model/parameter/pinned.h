#pragma once

#include "property.h"
#include "point_locator.h"
#include "coordinate2d.h"

namespace domain::plan::model::parameter
{
    template<typename TModel>
    struct pinned
    {
        pinned(point_locator_t p, double v, corecad::model::coordinate2d c)
            : coordinate { nullptr, c }
            , point { nullptr, p }
            , value { nullptr, v }
        {}

        corecad::model::property<corecad::model::coordinate2d, TModel> coordinate;
        corecad::model::property<point_locator_t, TModel> point;
        corecad::model::property<double, TModel> value;

        void reset_properties_updated()
        {
            coordinate.reset_updated();
            point.reset_updated();
            value.reset_updated();
        }

        void bind(TModel& parent)
        {
            coordinate.bind(parent);
            point.bind(parent);
            value.bind(parent);
        }
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const pinned<TModel>& p)
    {
        return os << "p:" << p.point << ": " << p.coordinate << "=" << p.value;
    }
}

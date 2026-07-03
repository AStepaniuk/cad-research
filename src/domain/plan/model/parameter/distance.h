#pragma once

#include "property.h"
#include "point_locator.h"
#include "coordinate2d.h"

namespace domain::plan::model::parameter
{
    template<typename TModel>
    struct distance
    {
        distance(point_locator_t f, point_locator_t t, double v, corecad::model::coordinate2d d)
            : direction { nullptr, d }
            , from { nullptr, f }
            , to { nullptr, t }
            , value { nullptr, v }
        {}

        corecad::model::property<corecad::model::coordinate2d, TModel> direction;

        corecad::model::property<point_locator_t, TModel> from;
        corecad::model::property<point_locator_t, TModel> to;

        corecad::model::property<double, TModel> value;

        void reset_properties_updated()
        {
            direction.reset_updated();
            from.reset_updated();
            to.reset_updated();
            value.reset_updated();
        }

        void bind(TModel& parent)
        {
            direction.bind(parent);
            from.bind(parent);
            to.bind(parent);
            value.bind(parent);
        }
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const distance<TModel>& d)
    {
        switch (d.direction)
        {
        case corecad::model::coordinate2d::x:
            os << "x";
            break;
        case corecad::model::coordinate2d::y:
            os << "y";
            break;
        default:
            os << "??";
            break;
        }

        return os << " from:" << d.from << " to:" << d.to << " val:" << d.value;
    }
}

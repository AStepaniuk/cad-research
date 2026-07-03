#pragma once

#include "property.h"
#include "point_locator.h"
#include "coordinate2d.h"

namespace domain::plan::model::parameter
{
    template<typename TModel>
    struct colinear
    {
        colinear(point_locator_t p1, point_locator_t p2, point_locator_t p3)
            : point1 { nullptr, p1 }
            , point2 { nullptr, p2 }
            , point3 { nullptr, p3 }
        {}

        corecad::model::property<point_locator_t, TModel> point1;
        corecad::model::property<point_locator_t, TModel> point2;
        corecad::model::property<point_locator_t, TModel> point3;

        void reset_properties_updated()
        {
            point1.reset_updated();
            point2.reset_updated();
            point3.reset_updated();
        }

        void bind(TModel& parent)
        {
            point1.bind(parent);
            point2.bind(parent);
            point3.bind(parent);
        }
    };

    template<typename TModel>
    std::ostream& operator<<(std::ostream& os, const colinear<TModel>& c)
    {
        return os << " p1:" << c.point1 << " p2:" << c.point2 << " p3:" << c.point3;
    }
}

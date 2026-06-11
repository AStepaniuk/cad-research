#pragma once

#include "model_base.h"
#include "property.h"
#include "point_locator.h"

namespace domain::plan::model::parameter
{
    enum class distance_direction { horizontal, vertical, diagonal };
    
    struct distance : public corecad::model::model_base<distance>
    {
        distance(point_locator_t f, point_locator_t t, double v, distance_direction d);
        distance(const distance& d);
        distance(distance&& d) noexcept;

        distance& operator=(const distance& other) = default;
        distance& operator=(distance&& other) noexcept = default;

        corecad::model::property<distance_direction, distance> direction;

        corecad::model::property<point_locator_t, distance> from;
        corecad::model::property<point_locator_t, distance> to;

        corecad::model::property<double, distance> value;

        void reset_properties_updated();
    };

    std::ostream& operator<<(std::ostream& os, const distance& d);
}

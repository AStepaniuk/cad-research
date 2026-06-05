#include "distance.h"

using namespace domain::plan::model::dimension;

distance::distance(point_locator_t f, point_locator_t t, double v, distance_direction d)
    : direction { this, d }
    , from { this, f }
    , to { this, t }
    , value { this, v }
{}

distance::distance(const distance &d)
    : direction { this, d.direction }
    , from { this, d.from }
    , to { this, d.to }
    , value { this, d.value }
{
}

domain::plan::model::dimension::distance::distance(distance &&d)
    : direction { this, d.direction }
    , from { this, d.from }
    , to { this, d.to }
    , value { this, d.value }
{
}

void distance::reset_properties_updated()
{
    direction.reset_updated();
    from.reset_updated();
    to.reset_updated();
    value.reset_updated();
}

std::ostream& operator<<(std::ostream &os, const distance &d)
{
    os << static_cast<const corecad::model::model_base<distance>&>(d) << ':';

    switch (d.direction)
    {
    case distance_direction::vertical:
        os << " v";
        break;
    case distance_direction::horizontal:
        os << " h";
        break;
    case distance_direction::diagonal:
        os << " d";
        break;        
    default:
        os << "unknown";
        break;
    }

    return os << " from:" << d.from << " to:" << d.to << " val:" << d.value;
}

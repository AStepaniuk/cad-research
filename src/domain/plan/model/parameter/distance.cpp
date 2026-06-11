#include "distance.h"

using namespace domain::plan::model::parameter;

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

distance::distance(distance &&d) noexcept
    : direction { this, std::move(d.direction) }
    , from { this, std::move(d.from) }
    , to { this, std::move(d.to) }
    , value { this, std::move(d.value) }
{
}

void distance::reset_properties_updated()
{
    direction.reset_updated();
    from.reset_updated();
    to.reset_updated();
    value.reset_updated();
}

std::ostream& domain::plan::model::parameter::operator<<(std::ostream &os, const distance &d)
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

    //return os << " from:" << d.from << " to:" << d.to << " val:" << d.value;
    return os;
}

#include <type_traits>

// Test 1: Is the variant itself fully noexcept movable?
static_assert(std::is_nothrow_move_constructible_v<domain::plan::model::parameter::point_locator_t>,
              "❌ STAGE 1 FAIL: point_locator_t variant itself is NOT nothrow movable!");

// Test 2: Is your custom property wrapper template noexcept movable?
static_assert(std::is_nothrow_move_constructible_v<corecad::model::property<domain::plan::model::parameter::point_locator_t, domain::plan::model::parameter::distance>>,
              "❌ STAGE 2 FAIL: The corecad::model::property wrapper is NOT nothrow movable!");

// Test 3: Is the entire distance struct legally nothrow movable?
static_assert(std::is_nothrow_move_constructible_v<domain::plan::model::parameter::distance>,
              "❌ STAGE 3 FAIL: The distance struct itself is NOT nothrow movable!");

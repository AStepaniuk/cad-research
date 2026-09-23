#pragma once

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>
#include <mp-units/ostream.h>

namespace corecad::model
{
    using length_mm_t = mp_units::quantity<mp_units::isq::length[mp_units::si::milli<mp_units::si::metre>]>;
    constexpr auto mm = mp_units::si::milli<mp_units::si::metre>;

    using angle_rad_t = mp_units::quantity<mp_units::isq::phase_angle[mp_units::si::radian]>;
    constexpr auto rad = mp_units::si::radian;
}

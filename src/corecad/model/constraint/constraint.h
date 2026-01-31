#pragma once

#include <variant>

#include "model_base.h"
#include "fixed.h"
#include "offset.h"

namespace corecad { namespace model { namespace constraint
{
    struct constraint : public model_base<constraint>
    {
        using instance_t = std::variant<fixed, offset>;

        constraint(instance_t i)
            : instance { std::move(i) }
        {}

        instance_t instance;
    };
}}}
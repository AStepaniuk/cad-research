#pragma once

#include <variant>

#include "model_base.h"

namespace corecad { namespace model { namespace constraint
{
    template<typename... TConstraints>
    struct constraint : public model_base<constraint<TConstraints...>>
    {
        using instance_t = std::variant<TConstraints...>;

        constraint(instance_t i)
            : instance { std::move(i) }
        {}

        instance_t instance;
    };
}}}
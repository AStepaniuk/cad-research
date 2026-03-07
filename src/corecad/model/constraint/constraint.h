#pragma once

#include <variant>

#include "model_base.h"
#include "registry.h"
#include "one_of.h"

namespace corecad { namespace model { namespace constraint
{
    template<template <typename> typename... TConstraints>
    struct constraint : public model_base<constraint<TConstraints...>>
    {
        using instance_t = std::variant<TConstraints<constraint>...>;

        instance_t instance;

        template<template <typename> typename TConstraint, typename... TArgs>
        requires IsOneOf<TConstraint<constraint>, TConstraints<constraint>...>
        static constraint create(TArgs&&... args)
        {
            constraint res { TConstraint<constraint> { std::forward<TArgs>(args)... } };
            res.bind_internal();

            return res;
        }

        constraint(const constraint& other)
            : instance { other.instance }
        {
            bind_internal();
        }

        constraint(constraint&& other)
            : instance { std::move(other.instance) }
        {
            bind_internal();
        }

        constraint& operator=(const constraint& other) = default;
        constraint& operator=(constraint&& other) = default;

    private:
        constraint(instance_t i)
            : instance { std::move(i) }
        {}
 
        void bind_internal()
        {
           std::visit([this](auto& impl) {
                impl.bind(*this);
            }, instance);
        }
    };
}}}
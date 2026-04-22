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
            : model_base<constraint<TConstraints...>> { other }
            , instance { other.instance }
        {
            bind_internal();
        }

        constraint(constraint&& other)
            : model_base<constraint<TConstraints...>> { other }
            , instance { std::move(other.instance) }
        {
            bind_internal();
        }

        constraint& operator=(const constraint& other) = default;
        constraint& operator=(constraint&& other) = default;

        void reset_properties_updated()
        {
           std::visit([this](auto& impl) {
                impl.reset_properties_updated();
            }, instance);
        }

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

    template<template <typename> typename FirstConstraint, template <typename> typename... TConstraints>
    std::ostream& operator<<(std::ostream& os, const constraint<FirstConstraint, TConstraints...>& c)
    {
        os << static_cast<const model_base<constraint<FirstConstraint, TConstraints...>>&>(c);

        std::visit([&os](auto& impl) {
            os << ' ' << impl;
        }, c.instance);

        return os;
    }
}}}

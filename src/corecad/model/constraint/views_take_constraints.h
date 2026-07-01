#pragma once

#include "views_take_variants.h"

namespace corecad { namespace model { namespace constraint { namespace views
{
    template <typename T>
    concept is_constraint_pair = requires(T p)
    {
        requires IsConstraint<decltype(p.second)>;
    };

    template<template <typename, typename> typename TConstraint>
    struct take_constraints_closure
    {
        template <std::ranges::viewable_range R>
        requires IsConstraint<std::ranges::range_value_t<R>>
        friend auto operator|(R&& r, take_constraints_closure)
        {
            using constraint_model_t = std::ranges::range_value_t<R>;
            using target_variant_t = typename constraint_model_t::template concrete_t<TConstraint>;

            return std::forward<R>(r)
                | std::views::transform([](auto&& c) -> decltype(auto) { 
                    return (std::forward<decltype(c)>(c).instance); 
                })
                | corecad::util::views::take_variants<target_variant_t>();
        }

        template <std::ranges::viewable_range R>
        requires is_constraint_pair<std::ranges::range_value_t<R>>
        friend auto operator|(R&& r, take_constraints_closure)
        {
            using constraint_model_t = std::ranges::range_value_t<R>::second_type;
            using target_variant_t = typename constraint_model_t::template concrete_t<TConstraint>;

            return std::forward<R>(r)
                | std::views::transform([](auto&& p) -> decltype(auto) { 
                    return (std::forward<decltype(p)>(p).second.instance); 
                })
                | corecad::util::views::take_variants<target_variant_t>();
        }
    };

    template<template <typename, typename> typename TConstraint>
    auto take_constraints()
    {
        return take_constraints_closure<TConstraint>{};
    }
}}}}

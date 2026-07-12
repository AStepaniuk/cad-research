#pragma once

#include "views_take_variants.h"

namespace corecad::model::views
{
    template <typename T>
    concept is_variant_model_pair = requires(T p)
    {
        requires IsVariantModel<std::remove_cvref_t<decltype(p.second)>>;
    };

    template<template <typename...> typename TInstance>
    struct take_model_variants_closure
    {
        template <std::ranges::viewable_range R>
        requires IsVariantModel<std::ranges::range_value_t<R>>
        friend auto operator|(R&& r, take_model_variants_closure)
        {
            using variant_model_t = std::remove_cvref_t<std::ranges::range_value_t<R>>;
            using target_variant_t = typename variant_model_t::template concrete_t<TInstance>;

            return std::forward<R>(r)
                | std::views::transform([](auto&& c) -> decltype(auto) { 
                    return (std::forward<decltype(c)>(c).instance); 
                })
                | corecad::util::views::take_variants<target_variant_t>();
        }

        template <std::ranges::viewable_range R>
        requires is_variant_model_pair<std::ranges::range_value_t<R>>
        friend auto operator|(R&& r, take_model_variants_closure)
        {
            using variant_model_t = std::remove_cvref_t<typename std::ranges::range_value_t<R>::second_type>;
            using target_variant_t = typename variant_model_t::template concrete_t<TInstance>;

            return std::forward<R>(r)
                | std::views::transform([](auto&& p) -> decltype(auto) { 
                    return (std::forward<decltype(p)>(p).second.instance); 
                })
                | corecad::util::views::take_variants<target_variant_t>();
        }
    };

    template<template <typename...> typename TInstance>
    auto take_model_variants()
    {
        return take_model_variants_closure<TInstance>{};
    }
}

#pragma once

#include <ranges>
#include <variant>
#include <concepts>

namespace corecad { namespace util { namespace views
{
    template <typename T>
    struct take_variants_closure
    {
        template <std::ranges::viewable_range R>
        friend auto operator|(R&& r, take_variants_closure)
        {
            return std::forward<R>(r)
                | std::views::filter([](const auto& v) {
                    return std::holds_alternative<T>(v);
                })
                | std::views::transform([](auto&& v) -> decltype(auto) {
                    return std::get<T>(std::forward<decltype(v)>(v));
                });
        }
    };

    template <typename T>
    auto take_variants()
    {
        return take_variants_closure<T>{};
    }
}}}

#pragma once

#include <type_traits>
#include <concepts>

namespace corecad { namespace util
{
    template<typename U, typename... Ts>
    struct unique_constructible
    {
        // Count how many types in the pack can be built from U
        static constexpr size_t count = (std::is_constructible_v<Ts, U> + ...);
        
        static_assert(count == 1, "Ambiguous or no Ts can be constructed from type U");

        // Extract the one type that matched
        using type = typename decltype([]<typename... Us>(Us...) {
            return [](auto... flags) { }(std::is_constructible_v<Us, U>...);
        }.template operator()<Ts...>())::type;
    };

    template<typename U, typename... Ts>
    using unique_constructible_t = unique_constructible<U, Ts...>::type;

    template <typename U, typename... Ts>
    concept ConstructibleIntoExactlyOne = (
        (static_cast<std::size_t>(std::constructible_from<Ts, U>) + ...) == 1
    );
}}

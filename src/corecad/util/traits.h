#pragma once

#include <type_traits>
#include <concepts>

namespace corecad { namespace util
{
    namespace impl
    {
        template<typename U, typename First, typename... Rest>
        struct find_constructible_type {
            using type = std::conditional_t<
                std::is_constructible_v<First, U>,
                First,
                typename find_constructible_type<U, Rest...>::type
            >;
        };

        template<typename U, typename Last>
        struct find_constructible_type<U, Last> {
            using type = Last;
        };
    }

    template<typename U, typename... Ts>
    struct unique_constructible
    {
        // Count how many types in the pack can be built from U
        static constexpr size_t count = (std::is_constructible_v<Ts, U> + ...);
        
        static_assert(count > 0, "No Ts is constructible from type U");
        static_assert(count == 1, "Ambiguity: Multiple Ts are constructible from type U");

        // Extract first type that matched
        using type = typename impl::find_constructible_type<U, Ts...>::type;
    };

    template<typename U, typename... Ts>
    using unique_constructible_t = unique_constructible<U, Ts...>::type;

    template <typename U, typename... Ts>
    concept ConstructibleIntoExactlyOne = (
        (static_cast<std::size_t>(std::constructible_from<Ts, U>) + ...) == 1
    );
}}

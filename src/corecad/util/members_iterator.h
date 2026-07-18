#pragma once

#include <type_traits>

namespace corecad::util
{
    // A concept to check if a model actually supports the static field visitor
    template <typename T>
    concept HasMembersMetadata = requires { T::members_metadata; };

    template <template<typename> typename Predicate, typename T, typename F>
    constexpr void visit_members(T&& instance, F&& func)
    {
        using U = std::remove_cvref_t<T>;
        
        static_assert(
            HasMembersMetadata<U>,
            "Error: The type passed to visit_members lacks metadata. "
            "Ensure the class defines a 'static constexpr auto members_metadata = std::make_tuple(...);' field."
        );

        if constexpr (HasMembersMetadata<U>)
        {
            std::apply([&](auto... member_ptrs) {
                ([&] {
                    auto& field = instance.*member_ptrs;
                    if constexpr (Predicate<std::remove_cvref_t<decltype(field)>>::value)
                    {
                        func(field);
                    }
                }(), ...);
            }, U::members_metadata);
        }
    }
}

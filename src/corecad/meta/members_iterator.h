#pragma once

#include <type_traits>

#include "member_info.h"

namespace corecad::meta
{
    // A concept to check if a model actually supports the static field visitor
    template <typename T>
    concept HasMembersMetadata = requires { T::metadata::members; };

    template <template<typename> typename Predicate, typename T, typename F>
    constexpr void visit_members(T&& instance, F&& func)
    {
        using U = std::remove_cvref_t<T>;
        
        static_assert(
            HasMembersMetadata<U>,
            "Error: The type passed to visit_members lacks the required metadata layout. "
            "Ensure the class defines a nested 'struct metadata { static constexpr auto members = std::make_tuple(...); };'."
        );

        if constexpr (HasMembersMetadata<U>)
        {
            std::apply([&](auto... descs) {
                ([&] {
                    auto& field = instance.*(descs.ptr);
                    using FieldType = std::remove_cvref_t<decltype(field)>;
                    if constexpr (Predicate<FieldType>::value)
                    {
                        func(field);
                    }
                }(), ...);
            }, U::metadata::members);
        }
    }
}

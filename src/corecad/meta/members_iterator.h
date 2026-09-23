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

    namespace impl
    {
        template <typename T>
        struct member_ptr_info {};

        template <typename TClass, typename TMember>
        struct member_ptr_info<TMember TClass::*>
        {
            using class_t = TClass;
            using member_t = TMember;
        };
    }

    template <template<typename> typename Predicate, typename T>
    constexpr size_t count_members()
    {
        using U = std::remove_cvref_t<T>;
        
        static_assert(
            HasMembersMetadata<U>,
            "Error: The type passed to visit_members lacks the required metadata layout. "
            "Ensure the class defines a nested 'struct metadata { static constexpr auto members = std::make_tuple(...); };'."
        );

        if constexpr (HasMembersMetadata<U>)
        {
            return std::apply([&](auto... descs) {
                return (0 + ... + ([]() {
                    using ptr_t = decltype(descs.ptr);

                    using field_raw_t = typename impl::member_ptr_info<ptr_t>::member_t;
                    using field_t = std::remove_cvref_t<field_raw_t>;

                    if constexpr (Predicate<field_t>::value)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }()));
            }, U::metadata::members);
        }
        else
        {
            return 0;
        }
    }
}

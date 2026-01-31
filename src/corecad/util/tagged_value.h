#pragma once

#include <iostream>
#include <functional>
#include <compare>

namespace corecad { namespace util
{
    template<typename T, typename Tag>
    struct tagged_value
    {
        using value_t = T;

        T value = T {};

        tagged_value() = default;
        explicit tagged_value(const T& val) : value(val) {}

        tagged_value(const tagged_value&) = default;
        tagged_value(tagged_value&&) = default;

        tagged_value& operator=(const tagged_value&) = default;
        tagged_value& operator=(tagged_value&&) = default;

        auto operator<=>(const tagged_value&) const = default;
    };

    template<typename T, typename Tag>
    std::ostream& operator<<(std::ostream& os, const tagged_value<T, Tag>& tagged)
    {
        return os << tagged.value;
    }
}}

namespace std {
    template<typename T, typename Tag>
    struct hash<corecad::util::tagged_value<T, Tag>> {
        size_t operator()(const corecad::util::tagged_value<T, Tag>& tv) const noexcept {
            // Use the standard hash for the underlying type T
            return std::hash<T>{}(tv.value);
        }
    };
}

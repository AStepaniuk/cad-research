#pragma once

#include <iostream>

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
    };

    template<typename T, typename Tag>
    std::ostream& operator<<(std::ostream& os, const tagged_value<T, Tag>& tagged)
    {
        return os << tagged.value;
    }
}}

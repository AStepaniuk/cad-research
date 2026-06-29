#pragma once

#include <type_traits>

#include "type_list.h"


namespace corecad::util {

    template <typename T, typename... Args>
    struct is_one_of
    {
        static constexpr bool value = (std::is_same_v<T, Args> || ...);
    };

    // Partial specialization for type_list
    template <typename T, typename... U>
    struct is_one_of<T, type_list<U...>>
    {
        static constexpr bool value = (std::is_same_v<T, U> || ...);
    };


    template <typename T, typename... U>
    concept IsOneOf = is_one_of<T, U...>::value;
}
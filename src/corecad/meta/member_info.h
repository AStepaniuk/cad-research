#pragma once

#include "compile_time_string.h"

namespace corecad::meta
{
    template<std::size_t N, typename TClass, typename TMember>
    struct member_info
    {
        compile_time_string<N> name;
        TMember TClass::*ptr;
    };

    template<std::size_t N, typename TClass, typename TMember>
    constexpr member_info<N, TClass, TMember> member(const char (&str)[N], TMember TClass::*ptr)
    {
        return member_info { compile_time_string<N> { str }, ptr };
    }
}

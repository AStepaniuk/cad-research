#pragma once

#include <string_view>

namespace corecad::meta
{
    template<typename TClass, typename TMember>
    struct member_info
    {
        std::string_view name;
        TMember TClass::*ptr;
    };

    template<typename TClass, typename TMember>
    constexpr member_info<TClass, TMember> member(std::string_view name, TMember TClass::*ptr)
    {
        return member_info { name, ptr };
    }
}

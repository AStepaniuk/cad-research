#pragma once

#include <functional>
#include <cstring>

namespace corecad { namespace util
{
    struct member_hasher
    {
        template <typename T, typename Class>
        std::size_t operator()(T Class::* ptr) const
        {
            unsigned char bytes[sizeof(ptr)];
            std::memcpy(bytes, &ptr, sizeof(ptr));
            
            std::size_t seed = 0;
            for (unsigned char b : bytes)
            {
                seed ^= std::hash<unsigned char>{}(b) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}}

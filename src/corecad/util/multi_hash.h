#pragma once

#include <functional>
#include <cstring>

namespace corecad { namespace util
{
    inline void hash_combine(std::size_t& seed, std::size_t v) noexcept
    {
        seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    struct member_hasher
    {
        template <typename T, typename Class>
        std::size_t operator()(T Class::* ptr) const noexcept
        {
            constexpr std::size_t chunk_count = (sizeof(ptr) + sizeof(std::size_t) - 1) / sizeof(std::size_t);
            
            std::size_t chunks[chunk_count]{}; 
            std::memcpy(chunks, &ptr, sizeof(ptr));
            
            std::size_t seed = 0;
            for (std::size_t i = 0; i < chunk_count; ++i)
            {
                hash_combine(seed, chunks[i]);
            }
            return seed;
        }
    };

    template <typename T>
    constexpr bool is_hash_noexcept() noexcept
    {
        if constexpr (std::is_member_pointer_v<T>)
        {
            return noexcept(member_hasher{}(std::declval<T>()));
        }
        else
        {
            return noexcept(std::hash<T>{}(std::declval<T>()));
        }
    }

    template <typename T>
    [[nodiscard]] constexpr std::size_t dispatch_hash(const T& val) noexcept(is_hash_noexcept<T>()) {
        if constexpr (std::is_member_pointer_v<T>)
        {
            return member_hasher{}(val);
        }
        else
        {
            return std::hash<T>{}(val);
        }
    }

    template <typename... Args>
    [[nodiscard]] std::size_t multi_hash(const Args&... args) noexcept(
        (noexcept(dispatch_hash(args)) && ...)
    ) {
        std::size_t seed = 0;
        (hash_combine(seed, dispatch_hash(args)), ...);
        return seed;
    }
}}

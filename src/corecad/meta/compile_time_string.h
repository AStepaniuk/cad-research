#pragma once

#include <array>

namespace corecad::meta
{
    template <std::size_t N1, std::size_t N2>
    struct compile_time_string
    {
        std::array<char, N1 + N2 + 1> buffer{};

        constexpr compile_time_string(std::string_view s1, std::string_view s2)
        {
            std::size_t idx = 0;
            for (char c : s1) buffer[idx++] = c;
            for (char c : s2) buffer[idx++] = c;
            buffer[idx] = '\0';
        }

        constexpr std::string_view view() const
        {
            return {buffer.data(), buffer.size() - 1};
        }
    };
}

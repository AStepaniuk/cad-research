#pragma once

#include <array>
#include <string_view>

namespace corecad::meta
{
    template<std::size_t N>
    struct compile_time_string
    {
        std::array<char, N> buffer{};

        constexpr compile_time_string(const char (&str)[N])
        {
            std::copy(str, str + N, buffer.begin());
        }

        constexpr compile_time_string(const std::array<char, N>& initial_buffer) 
            : buffer(initial_buffer) 
        {}

        constexpr std::string_view view() const
        {
            return {buffer.data(), N - 1}; 
        }

        constexpr operator std::string_view() const
        { 
            return view();
        }
    };

    
    template<std::size_t N>
    compile_time_string(const char (&)[N]) -> compile_time_string<N>;


    template <std::size_t N1, std::size_t N2>
    constexpr auto operator+(compile_time_string<N1> lhs, compile_time_string<N2> rhs)
    {
        constexpr std::size_t combined_size = (N1 - 1) + (N2 - 1) + 1;
        std::array<char, combined_size> new_buffer{};

        std::size_t idx = 0;
        
        for (std::size_t i = 0; i < N1 - 1; ++i)
        {
            new_buffer[idx++] = lhs.buffer[i];
        }
        
        for (std::size_t i = 0; i < N2; ++i)
        {
            new_buffer[idx++] = rhs.buffer[i];
        }

        return compile_time_string<combined_size>{new_buffer};
    }

    template <std::size_t N1, std::size_t N2>
    constexpr auto operator+(compile_time_string<N1> lhs, const char (&rhs)[N2])
    {
        return lhs + compile_time_string(rhs);
    }

    template <std::size_t N1, std::size_t N2>
    constexpr auto operator+(const char (&lhs)[N1], compile_time_string<N2> rhs)
    {
        return compile_time_string(lhs) + rhs;
    }
}

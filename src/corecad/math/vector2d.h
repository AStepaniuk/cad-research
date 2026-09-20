#pragma once

#include <cstddef>
#include <optional>
#include <type_traits>
#include <mp-units/framework.h>
#include <mp-units/math.h>
#include <mp-units/systems/si.h>

namespace corecad::math
{
    template <typename TValue>
    struct vector2d
    {
        TValue x;
        TValue y;

        constexpr vector2d& operator+=(vector2d<TValue> rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
        constexpr vector2d& operator-=(vector2d<TValue> rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
        
        template <typename TOperand>
        constexpr vector2d& operator*=(TOperand op) noexcept { x *= op; y *= op; return *this; }
        
        template <typename TOperand>
        constexpr vector2d& operator/=(TOperand op) noexcept { x /= op; y /= op; return *this; }
    };

    template<typename T>
    inline constexpr bool is_mp_quantity_v = false;

    template<auto R, typename Rep>
    inline constexpr bool is_mp_quantity_v<mp_units::quantity<R, Rep>> = true;


    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator+(vector2d<T> lhs, vector2d<U> rhs) noexcept
        -> vector2d<decltype(std::declval<T>() + std::declval<U>())>
    {
        return { lhs.x + rhs.x, lhs.y + rhs.y };
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator-(vector2d<T> lhs, vector2d<U> rhs) noexcept
        -> vector2d<decltype(std::declval<T>() - std::declval<U>())>
    {
        return { lhs.x - rhs.x, lhs.y - rhs.y };
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator*(vector2d<T> lhs, U op) noexcept 
        -> vector2d<decltype(std::declval<T>() * std::declval<U>())>
    {
        return { lhs.x * op, lhs.y * op };
    }

    template<typename U, typename T>
    [[nodiscard]] constexpr auto operator*(U op, vector2d<T> rhs) noexcept 
        -> vector2d<decltype(std::declval<U>() * std::declval<T>())>
    {
        return { op * rhs.x, op * rhs.y };
    }

    template<typename T, typename U>
    [[nodiscard]] constexpr auto operator/(vector2d<T> lhs, U op) noexcept 
        -> vector2d<decltype(std::declval<T>() / std::declval<U>())>
    {
        return { lhs.x / op, lhs.y / op };
    }

    template <typename T, typename U>
    [[nodiscard]] constexpr auto cross_product(vector2d<T> a, vector2d<U> b) noexcept
        -> decltype(a.x * b.y - a.y * b.x)
    {
        return a.x * b.y - a.y * b.x;
    }

    template <typename T>
    [[nodiscard]] constexpr auto angle(vector2d<T> v) noexcept
    {
        if constexpr (is_mp_quantity_v<T>)
        {
            return mp_units::si::atan2(v.y, v.x);
        }
        else
        {
            using std::atan2;
            return atan2(v.y, v.x);
        }
    }

    template <typename T>
    [[nodiscard]] constexpr auto length(vector2d<T> v) noexcept
    {
        if constexpr (is_mp_quantity_v<T>)
        {
            return mp_units::hypot(v.y, v.x);
        }
        else
        {
            using std::hypot;
            return hypot(v.y, v.x);
        }
    }

    template <typename T>
    [[nodiscard]] constexpr auto normalize(vector2d<T> v) noexcept
    {
        return v / length(v);
    }

    template <typename T>
    [[nodiscard]] constexpr vector2d<T> rotate_left(vector2d<T> v) noexcept
    {
        return vector2d<T> { v.y, -v.x };
    }

    template <typename T>
    [[nodiscard]] constexpr std::optional<vector2d<T>> lines_intersection(
        vector2d<T> p1, vector2d<T> v1,
        vector2d<T> p2, vector2d<T> v2
    ) noexcept
    {
        const auto det = cross_product(v1, v2);

        if constexpr (is_mp_quantity_v<T>)
        {
            using Rep = typename T::rep;

            if constexpr (std::is_floating_point_v<Rep>)
            {
                if (mp_units::abs(det) < static_cast<Rep>(0.0001) * det.unit)
                {
                    return std::nullopt;
                }
            }
            else
            {
                if (det == 0 * det.unit)
                {
                    return std::nullopt;
                }
            }
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            if (std::abs(det) < static_cast<T>(0.0001))
            {
                return std::nullopt;
            }
        }
        else
        {
            if (det == static_cast<T>(0))
            {
                return std::nullopt;
            }
        }

        const vector2d<T> p2_minus_p1 { p2.x - p1.x, p2.y - p1.y };
        const auto t = cross_product(p2_minus_p1, v2) / det;

        return vector2d<T>{ p1.x + t * v1.x, p1.y + t * v1.y };
    }

    template <typename T, typename U>
    [[nodiscard]] constexpr auto point_offset(vector2d<T> start, vector2d<T> end, U offset) noexcept
    {
        const auto nrv = rotate_left(normalize(end - start));
        
        return start + nrv * offset;
    }
}

#pragma once

#include <string_view>
#include <type_traits>

namespace corecad::meta
{
    namespace detail
    {
        template <typename U>
        static consteval std::string_view raw_name()
        {
            #if defined(__clang__)
                constexpr std::string_view func = __PRETTY_FUNCTION__;
                constexpr auto start = func.find("[U = ") + 5;
                constexpr auto end = func.rfind("]");
                
                static_assert(start != std::string_view::npos && end != std::string_view::npos, 
                              "Unsupported Clang __PRETTY_FUNCTION__ format");
                return func.substr(start, end - start);

            #elif defined(__GNUC__)
                constexpr std::string_view func = __PRETTY_FUNCTION__;
                constexpr auto start = func.find("[with U = ") + 10;
                constexpr auto bracket_end = func.rfind("]");
                
                static_assert(start != std::string_view::npos && bracket_end != std::string_view::npos, 
                            "Unsupported GCC __PRETTY_FUNCTION__ format");

                constexpr auto semi = func.find(";", start);
                constexpr auto end = (semi != std::string_view::npos && semi < bracket_end) ? semi : bracket_end;

                return func.substr(start, end - start);

            #elif defined(_MSC_VER)
                constexpr std::string_view func = __FUNCSIG__;
                constexpr std::string_view prefix = "raw_name<"; 
                constexpr auto start = func.find(prefix) + prefix.size();
                constexpr auto end = func.rfind(">(void)");
                
                std::string_view raw = func.substr(start, end - start);
                if (raw.starts_with("struct ")) raw = raw.substr(7);
                else if (raw.starts_with("class ")) raw = raw.substr(6);
                else if (raw.starts_with("enum "))  raw = raw.substr(5);
                static_assert(start != std::string_view::npos && end != std::string_view::npos, 
                              "Unsupported MSVC __FUNCSIG__ format");
                return raw;

            #else
                return "Unsupported Compiler";
            #endif
        }

        template <typename CleanT>
        struct type_meta_info_impl
        {
            static consteval std::string_view name()
            {
                return raw_name<CleanT>();
            }
        };
    }

    template <typename T>
    struct type_meta_info
    {
        static consteval std::string_view name()
        {
            return detail::type_meta_info_impl<std::remove_cvref_t<T>>::name();
        }
    };

    template <typename T>
    constexpr std::string_view type_name()
    {
        if constexpr (requires { T::metadata::type_name; })
        {
            return T::metadata::type_name;
        }
        else
        {
            return type_meta_info<T>::name();
        }
    }
}

#pragma once

#include <variant>

namespace corecad::util
{
    template <typename... Ts>
    struct type_list
    {
        using variant_t = std::variant<Ts...>;
    };


    template <typename T, typename TList> struct index_of;

    template <typename T, typename... Ts>
    struct index_of<T, type_list<Ts...>>
    {
        static constexpr std::size_t value = []() {
            constexpr std::array<bool, sizeof...(Ts)> matches = { std::is_same_v<T, Ts>... };
            for (std::size_t i = 0; i < matches.size(); ++i)
            {
                if (matches[i]) return i;
            }
            return matches.size();
        }();
        static_assert(value < sizeof...(Ts), "Type not found in type_list.");
    };


    template <typename T>
    struct is_type_list : std::false_type {};

    template <typename... Ts>
    struct is_type_list<type_list<Ts...>> : std::true_type {};

    template <typename T>
    concept IsTypeList = is_type_list<T>::value;


    template <typename TList, template <typename> typename Predicate>
    struct all_elements_are : std::false_type {};

    template <typename... Ts, template <typename> typename Predicate>
    struct all_elements_are<type_list<Ts...>, Predicate> 
        : std::bool_constant<(Predicate<std::remove_cvref_t<Ts>>::value && ...)> {};

    template <typename TList, template <typename> typename Predicate>
    concept AllElementsAre = all_elements_are<TList, Predicate>::value;
}

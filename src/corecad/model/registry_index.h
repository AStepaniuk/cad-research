#pragma once

#include "tagged_value.h"
#include "type_meta_info.h"
#include "compile_time_string.h"

namespace corecad::model
{
    template <typename Tag>
    using registry_index_t = util::tagged_value<long, Tag>;

    template <typename Tag>
    registry_index_t<Tag>& operator++(registry_index_t<Tag>& index)
    {
        ++index.value;
        return index;
    }

    template <typename Tag>
    registry_index_t<Tag> operator++(registry_index_t<Tag>& index, int)
    {
        auto temp = index;
        ++index.value;
        return temp;
    }
      
    template <typename T>
    struct is_registry_index : std::false_type {};

    template <typename Tag>
    struct is_registry_index<registry_index_t<Tag>> : std::true_type {};

    template <typename T>
    concept IsRegistryIndex = is_registry_index<std::remove_cvref_t<T>>::value;
}

namespace corecad::util::detail
{
    template <typename Tag>
    struct type_meta_info_impl<corecad::model::registry_index_t<Tag>>
    {
        static consteval std::string_view name()
        {
            // route back through the main public interface
            constexpr std::string_view t_name = corecad::util::type_meta_info<Tag>::name();
            constexpr std::string_view suffix = "::index_t";
            static constexpr auto str = compile_time_string<t_name.size(), suffix.size()>(t_name, suffix);
            return str.view();
        }
    };
}

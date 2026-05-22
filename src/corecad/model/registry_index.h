#pragma once

#include "tagged_value.h"

namespace corecad { namespace model
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
}}

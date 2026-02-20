#pragma once

#include "tagged_value.h"

namespace corecad { namespace model
{
    template <typename Tag>
    using registry_index_t = util::tagged_value<long, Tag>;

    template <typename Tag>
    registry_index_t<Tag>& operator++(registry_index_t<Tag>& tagged)
    {
        ++tagged.value;
        return tagged;
    }

    template <typename Tag>
    registry_index_t<Tag> operator++(registry_index_t<Tag>& tagged, int)
    {
        auto temp = tagged;
        ++tagged.value;
        return temp;
    }
}}

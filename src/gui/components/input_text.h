#pragma once

#include <imgui.h>
#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>

#include "property.h"

namespace gui::components
{
    namespace impl
    {
        bool input_text(
            const char* label, char* buffer, size_t buffer_size,
            const void* unique_id_handle
        );
    }

    inline bool input_text(const char* label, char* buffer, size_t buffer_size)
    {
        return impl::input_text(label, buffer, buffer_size, static_cast<const void*>(buffer));
    }

    template <typename TModel>
    bool input_text(const char* label, corecad::model::property<std::string, TModel>& property)
    {
        std::string current_val = property.val();
        
        std::vector<char> dynamic_scratch(std::max<size_t>(256, current_val.size() + 64), '\0');
        std::copy(current_val.begin(), current_val.end(), dynamic_scratch.begin());

        bool value_changed = impl::input_text(
            label, 
            dynamic_scratch.data(), 
            dynamic_scratch.size(), 
            static_cast<const void*>(&property)
        );
        
        if (value_changed)
        {
            property = std::string(dynamic_scratch.data());
        }

        return value_changed;
    }
}

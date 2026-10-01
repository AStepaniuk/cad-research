#pragma once

#include <imgui.h>
#include <algorithm>
#include <type_traits>
#include <cstdio>

#include "property.h"
#include "component_utils.h"

namespace gui::components
{
    namespace impl
    {
        template <typename T>
        bool input_scalar_clamped(
            const char* label, T& value, 
            T min_val, T max_val, 
            const char* format,
            T step,
            const void* unique_id_handle
        )
        {
            static_assert(std::is_arithmetic_v<T>, "input_scalar_clamped only supports primitive numeric types.");

            constexpr ImGuiDataType data_type = impl::get_imgui_data_type<T>();
            T raw_val = value;

            constexpr float label_column_width = 220.0f;

            ImVec2 start_cursor_pos = ImGui::GetCursorPos();

            char label_click_zone_id[64];
            std::snprintf(label_click_zone_id, sizeof(label_click_zone_id), "##ClickZone_%p", unique_id_handle);
            
            if (ImGui::InvisibleButton(
                label_click_zone_id,
                ImVec2(label_column_width - ImGui::GetStyle().ItemSpacing.x, ImGui::GetFrameHeight())
            ))
            {
                ImGui::SetKeyboardFocusHere(0);
            }

            ImGui::SetCursorPos(start_cursor_pos);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted(label);

            ImGui::SameLine(label_column_width);
            ImGui::SetNextItemWidth(-FLT_MIN);

            char hidden_id[32];
            std::snprintf(hidden_id, sizeof(hidden_id), "##Scalar_%p", unique_id_handle);

            bool value_changed = false;
            T* step_ptr = (step == T{}) ? nullptr : &step;
            if (ImGui::InputScalar(hidden_id, data_type, &raw_val, step_ptr, nullptr, format))
            {
                if (raw_val < min_val) raw_val = min_val;
                if (raw_val > max_val) raw_val = max_val;

                value = raw_val;
                value_changed = true;
            }

            return value_changed;
        }
    }

    template <typename T>
    bool input_scalar_clamped(
        const char* label, T& value, 
        T min_val, T max_val, 
        const char* format,
        T step = {}
    )
    {
        return impl::input_scalar_clamped(label, value, min_val, max_val, format, step, static_cast<const void*>(&value));
    }

    template <typename T, typename TModel>
    bool input_scalar_clamped(
        const char* label, corecad::model::property<T, TModel>& property, 
        T min_val, T max_val, 
        const char* format,
        T step = {}
    )
    {
        T raw_val = property.val();

        auto res = impl::input_scalar_clamped(label, raw_val, min_val, max_val, format, step, static_cast<const void*>(&property));
        if (res)
        {
            property = raw_val;
        }

        return res;
    }
}

#pragma once

#include <string>
#include <imgui.h>

namespace gui::components
{
    namespace impl
    {
        bool contains_case_insensitive(const std::string& haystack, const std::string& needle);

        template<typename T>
        constexpr ImGuiDataType get_imgui_data_type()
        {
            if constexpr (std::is_same_v<T, float>)          return ImGuiDataType_Float;
            if constexpr (std::is_same_v<T, double>)         return ImGuiDataType_Double;
            if constexpr (std::is_same_v<T, int>)            return ImGuiDataType_S32;
            if constexpr (std::is_same_v<T, unsigned int>)   return ImGuiDataType_U32;
            if constexpr (std::is_same_v<T, long long>)      return ImGuiDataType_S64;
            if constexpr (std::is_same_v<T, unsigned long long>) return ImGuiDataType_U64;
            return ImGuiDataType_Double;
        }

        struct btn_colors
        {
            ImVec4 base;
            ImVec4 hovered;
            ImVec4 active;
        };

        btn_colors get_warning_button_colors();
    }
}
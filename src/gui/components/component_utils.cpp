#include "component_utils.h"

#include <algorithm>

bool gui::components::impl::contains_case_insensitive(const std::string &haystack, const std::string &needle)
{
    if (needle.empty()) return true;
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
    );
    return it != haystack.end();
}

gui::components::impl::btn_colors gui::components::impl::get_warning_button_colors()
{
    ImVec4 base_red = ImGui::GetStyle().Colors[ImGuiCol_PlotLinesHovered];
    
    ImVec4 text_color = ImGui::GetStyle().Colors[ImGuiCol_Text];
    bool is_light_theme = (text_color.x + text_color.y + text_color.z < 1.5f);

    ImVec4 hovered_red = base_red;
    ImVec4 active_red = base_red;

    if (is_light_theme)
    {
        // Light Theme: Hover deepens the red; Active darkens it further
        hovered_red.x *= 0.85f; hovered_red.y *= 0.85f; hovered_red.z *= 0.85f;
        active_red.x  *= 0.70f; active_red.y  *= 0.70f; active_red.z  *= 0.70f;
    }
    else
    {
        // Dark Theme: Hover brightens the red; Active intensifies or deepens it
        hovered_red.x = std::min(hovered_red.x * 1.20f, 1.0f);
        hovered_red.y = std::min(hovered_red.y * 1.20f, 1.0f);
        hovered_red.z = std::min(hovered_red.z * 1.20f, 1.0f);
        
        active_red.x *= 0.85f; active_red.y *= 0.85f; active_red.z *= 0.85f;
    }

    return btn_colors
    {
        .base = base_red,
        .hovered = hovered_red,
        .active = active_red
    };
}

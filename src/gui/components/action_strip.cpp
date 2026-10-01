#include "action_strip.h"

#include "component_utils.h"

int gui::components::action_strip(std::initializer_list<button_meta> buttons)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    int clicked_index = -1;
    int current_index = 0;

    bool enter_pressed = ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter);
    bool block_enter = ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId);

    bool is_first_item = true;

    for (const auto& btn : buttons)
    {
        if (!is_first_item)
        {
            ImGui::SameLine();
        }
        is_first_item = false;

        size_t custom_styles_pushed = 0;            
        if (btn.style == button_style::warning)
        {
            auto red_colors = impl::get_warning_button_colors();

            ImGui::PushStyleColor(ImGuiCol_Button, red_colors.base);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, red_colors.hovered);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, red_colors.active);
            custom_styles_pushed = 3;
        }
        else if (btn.is_default)
        {
            // Highlight the default action button using the active theme's focused brand color tones
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            custom_styles_pushed = 1;
        }

        float calculated_width = ImGui::CalcTextSize(btn.title).x + ImGui::GetStyle().FramePadding.x * 3.0f;
        if (calculated_width < 100.0f) calculated_width = 100.0f; 

        if (ImGui::Button(btn.title, ImVec2(calculated_width, 0.0f)) || (btn.is_default && enter_pressed && !block_enter))
        {
            clicked_index = current_index;
        }

        if (custom_styles_pushed > 0)
        {
            ImGui::PopStyleColor(custom_styles_pushed);
        }

        current_index++;
    }

    return clicked_index;
}

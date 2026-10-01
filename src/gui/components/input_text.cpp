#include "input_text.h"

bool gui::components::impl::input_text(const char *label, char *buffer, size_t buffer_size, const void *unique_id_handle)
{
    constexpr float label_column_width = 220.0f;
    ImVec2 start_cursor_pos = ImGui::GetCursorPos();

    char label_click_zone_id[64];
    std::snprintf(label_click_zone_id, sizeof(label_click_zone_id), "##TextClickZone_%p", unique_id_handle);
    
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
    std::snprintf(hidden_id, sizeof(hidden_id), "##TextInputField_%p", unique_id_handle);

    return ImGui::InputText(hidden_id, buffer, buffer_size);
}

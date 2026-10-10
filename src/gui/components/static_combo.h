#pragma once

#include <string>
#include <vector>
#include <optional>
#include <cstdio>

#include <imgui.h>

#include "property.h"

namespace gui::components
{
    template <typename T>
    class static_combo
    {
    public:
        struct item_entry
        {
            T value;
            std::string display_name;
        };

        static_combo(const std::vector<item_entry>& items)
            : _items { items }
        {
        };

        template <typename TModel>
        void render(
            const char* label,
            corecad::model::property<T, TModel>& prop,
            float override_width = -1.0f
        )
        {
            constexpr float label_column_width = 220.0f;
            ImVec2 start_cursor_pos = ImGui::GetCursorPos();

            char label_click_zone_id[64];
            std::snprintf(label_click_zone_id, sizeof(label_click_zone_id), "##ComboClickZone_%p", static_cast<const void*>(&prop));
            
            bool open_combo_via_click = false;
            if (ImGui::InvisibleButton(
                label_click_zone_id,
                ImVec2(label_column_width - ImGui::GetStyle().ItemSpacing.x, ImGui::GetFrameHeight())
            ))
            {
                open_combo_via_click = true;
            }

            ImGui::SetCursorPos(start_cursor_pos);
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted(label);

            ImGui::SameLine(label_column_width);
            
            if (override_width > 0.0f)
            {
                ImGui::SetNextItemWidth(override_width);
            }
            else
            {
                ImGui::SetNextItemWidth(-FLT_MIN);
            }

            const char* selection_text = "";
            for (const auto& item : _items)
            {
                if (item.value == prop)
                {
                    selection_text = item.display_name.c_str();
                    break;
                }
            }

            char hidden_id[64];
            std::snprintf(hidden_id, sizeof(hidden_id), "##StaticComboField_%p", static_cast<const void*>(&prop));

            if (open_combo_via_click)
            {
                ImGui::SetNextItemOpen(true, ImGuiCond_Always);
            }

            if (ImGui::BeginCombo(hidden_id, selection_text))
            {
                for (const auto& item : _items)
                {
                    bool is_selected = (item.value == prop);

                    if (ImGui::Selectable(item.display_name.c_str(), is_selected))
                    {
                        prop = item.value;
                        ImGui::CloseCurrentPopup();
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        std::vector<item_entry> _items;
        const char* _default_selection_text;
    };
}

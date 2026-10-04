#pragma once

#include <string>
#include <vector>
#include <optional>

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
            if (override_width > 0.0f)
            {
                ImGui::SetNextItemWidth(override_width);
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

            if (ImGui::BeginCombo(label, selection_text))
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

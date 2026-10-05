#pragma once

#include <string>
#include <vector>
#include <utility>
#include <imgui.h>

#include "property.h"

namespace gui::components
{
    template <typename TEnum, typename TModel>
    class static_combo_cell
    {
    public:
        struct item_entry
        {
            std::string display_name;
            TEnum enum_value;
        };

        using prop_ptr_t = corecad::model::property<TEnum, TModel> TModel::*;

        static_combo_cell(
            prop_ptr_t prop_ptr,
            std::vector<item_entry> items,
            const char* default_selection_text = "<Unassigned>"
        )
            : _prop_ptr { prop_ptr }
            , _items { std::move(items) }
            , _default_selection_text { default_selection_text }
        {
        }

        bool render(TModel& item)
        {
            auto& prop = item.*_prop_ptr;
            TEnum current_val = prop.val();
            bool changed = false;

            const char* selection_text = _default_selection_text;
            for (const auto& entry : _items)
            {
                if (entry.enum_value == current_val)
                {
                    selection_text = entry.display_name.c_str();
                    break;
                }
            }

            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::BeginCombo("##StaticComboCell", selection_text))
            {
                for (const auto& entry : _items)
                {
                    bool is_selected = (entry.enum_value == current_val);

                    if (ImGui::Selectable(entry.display_name.c_str(), is_selected))
                    {
                        prop = entry.enum_value;
                        changed = true;
                        ImGui::CloseCurrentPopup();
                    }

                    if (is_selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            return changed;
        }

    private:
        prop_ptr_t _prop_ptr;
        std::vector<item_entry> _items;
        const char* _default_selection_text;
    };
}

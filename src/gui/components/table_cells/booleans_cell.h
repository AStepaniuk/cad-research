#pragma once

#include <string>
#include <vector>
#include <imgui.h>

#include "property.h"

namespace gui::components
{
    template <typename TModel>
    class booleans_cell
    {
    public:
        using prop_ptr_t = corecad::model::property<bool, TModel> TModel::*;

        struct flag_definition
        {
            std::string label;
            prop_ptr_t prop_ptr;
            std::string tooltip = "";
        };

        booleans_cell(std::vector<flag_definition> flags)
            : _flags { std::move(flags) }
        {
        }

        bool render(TModel& item)
        {
            bool changed = false;

            ImGui::BeginGroup();

            for (size_t i = 0; i < _flags.size(); ++i)
            {
                const auto& flag = _flags[i];
                
                auto& prop = item.*(flag.prop_ptr);
                bool value = prop.val();

                std::string unique_label_id = flag.label + "##BoolCellFlag_" + std::to_string(i);

                if (ImGui::Checkbox(unique_label_id.c_str(), &value))
                {
                    prop = value;
                    changed = true;
                }

                if (!flag.tooltip.empty() && ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("%s", flag.tooltip.c_str());
                }

                if (i < _flags.size() - 1)
                {
                    ImGui::SameLine(0.0f, 8.0f); // Spacing gap between separate checkbox entries
                }
            }

            ImGui::EndGroup();

            return changed;
        }

    private:
        std::vector<flag_definition> _flags;
    };
}

#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <imgui.h>

#include "property.h"
#include "component_utils.h"

namespace gui::components
{
    template <typename TModel, typename TRegistryLookup>
    class registry_combo_cell
    {
    public:
        using registry_t = TRegistryLookup;
        using reg_model_t = typename registry_t::data_t;
        using index_t = typename registry_t::index_t;

        using row_prop_ptr_t = corecad::model::property<index_t, TModel> TModel::*;        
        using reg_title_ptr_t = corecad::model::property<std::string, reg_model_t> reg_model_t::*;

        struct combo_item_entry
        {
            index_t index;
            std::string display_name;
        };

        registry_combo_cell(
            row_prop_ptr_t row_prop_ptr,
            const TRegistryLookup& registry,
            reg_title_ptr_t reg_title_ptr
        )
            : _row_prop_ptr { row_prop_ptr }
            , _registry { registry }
            , _reg_title_ptr { reg_title_ptr }
        {
            refresh_list();
        }

        void refresh_list()
        {
            _cached_items.clear();

            for (const auto& [_, reg_item] : _registry)
            {
                _cached_items.push_back({ reg_item.index, (reg_item.*_reg_title_ptr).val() });
            }

            std::sort(_cached_items.begin(), _cached_items.end(), 
                [](const auto& a, const auto& b) {
                    return std::lexicographical_compare(
                        a.display_name.begin(), a.display_name.end(),
                        b.display_name.begin(), b.display_name.end(),
                        [](char ch1, char ch2) {
                            return std::tolower(static_cast<unsigned char>(ch1)) < 
                                   std::tolower(static_cast<unsigned char>(ch2));
                        }
                    );
                }
            );
        }

        bool render(TModel& item)
        {
            auto& prop = item.*_row_prop_ptr;
            index_t current_val = prop.val();
            bool changed = false;

            const char* selection_text = "";
            for (const auto& entry : _cached_items)
            {
                if (entry.index == current_val)
                {
                    selection_text = entry.display_name.c_str();
                    break;
                }
            }

            ImGui::SetNextItemWidth(-FLT_MIN); 
            if (ImGui::BeginCombo("##RegistryCellCombo", selection_text))
            {
                if (ImGui::IsWindowAppearing())
                {
                    ImGui::SetKeyboardFocusHere();
                }

                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputTextWithHint("##CellComboSearchInput", "Filter...", _search_buffer, sizeof(_search_buffer));
                ImGui::Separator();

                ImGui::BeginChild("RegistryCellComboScroll", ImVec2(0, 200.0f), ImGuiChildFlags_None, ImGuiWindowFlags_NoMove);

                std::string search_query(_search_buffer);
                bool any_items_visible = false;

                for (const auto& entry : _cached_items)
                {
                    if (impl::contains_case_insensitive(entry.display_name, search_query))
                    {
                        any_items_visible = true;
                        bool is_selected = (entry.index == current_val);

                        if (ImGui::Selectable(entry.display_name.c_str(), is_selected))
                        {
                            prop = entry.index;
                            changed = true;
                            _search_buffer[0] = '\0';
                            ImGui::CloseCurrentPopup();
                        }

                        if (is_selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }

                if (!any_items_visible)
                {
                    ImGui::TextDisabled("%s", "No matches found.");
                }

                ImGui::EndChild();
                ImGui::EndCombo();
            }
            else if (_search_buffer[0] != '\0')
            {
                // Clear out search query caches if user drops focus without changing selection
                _search_buffer[0] = '\0';
            }

            return changed;
        }

    private:
        row_prop_ptr_t _row_prop_ptr;
        const TRegistryLookup& _registry;
        reg_title_ptr_t _reg_title_ptr;

        std::vector<combo_item_entry> _cached_items;
        char _search_buffer[128] = "";
    };
}

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cctype>
#include <imgui.h>

namespace gui::components
{
    namespace impl
    {
        bool contains_case_insensitive(const std::string& haystack, const std::string& needle);
    }

    template <typename T>
    class searchable_combo
    {
    public:
        struct item_entry
        {
            T value;
            std::string display_name;
        };

        searchable_combo() = default;

        // Renders the searchable combo box. Returns true if the user changed the selection.
        bool render(
            const char* label,
            const std::string& current_selection_text,
            const std::vector<item_entry>& items,
            T& out_selected_value, 
            float override_width = -1.0f
        )
        {
            bool selection_changed = false;

            if (override_width > 0.0f)
            {
                ImGui::SetNextItemWidth(override_width);
            }

            if (ImGui::BeginCombo(label, current_selection_text.c_str()))
            {
                // Auto-focus the search input element the single frame the dropdown first appears
                if (ImGui::IsWindowAppearing())
                {
                    ImGui::SetKeyboardFocusHere();
                }

                // Render Search Bar
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputTextWithHint("##ComboSearchInput", "Type to filter...", _search_buffer, sizeof(_search_buffer));
                ImGui::Separator();

                // Scrollable container bounding area protecting high collection counts
                ImGui::BeginChild("SearchableComboDropdownScroll", ImVec2(0, 200.0f), ImGuiChildFlags_None, ImGuiWindowFlags_NoMove);

                std::string search_query(_search_buffer);
                bool any_items_visible = false;

                for (const auto& item : items)
                {
                    if (impl::contains_case_insensitive(item.display_name, search_query))
                    {
                        any_items_visible = true;
                        bool is_selected = (item.display_name == current_selection_text);

                        if (ImGui::Selectable(item.display_name.c_str(), is_selected))
                        {
                            out_selected_value = item.value;
                            selection_changed = true;
                            _search_buffer[0] = '\0'; // Purge criteria strings
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
                    ImGui::TextDisabled("%s", "No matching options found.");
                }

                ImGui::EndChild();
                ImGui::EndCombo();
            }
            else if (_search_buffer[0] != '\0')
            {
                // Clean up string caches if the user dropped focus without picking anything
                _search_buffer[0] = '\0';
            }

            return selection_changed;
        }

    private:
        char _search_buffer[128] = "";
    };
}

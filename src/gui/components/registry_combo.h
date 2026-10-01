#pragma once

#include <imgui.h>
#include <optional>
#include <vector>
#include <string>
#include <algorithm>

#include "property.h"
#include "searchable_combo.h"

namespace gui::components
{
    template <typename TRegistry>
    class registry_combo
    {
    public:
        using registry_t = TRegistry;
        using model_t = typename registry_t::data_t;
        using index_t = typename registry_t::index_t;

        registry_combo(const TRegistry& registry, corecad::model::property<std::string, model_t> model_t::* title_ptr)
            : _registry { registry }
            , _title_ptr { title_ptr }
        {}

        void refresh()
        {
            _combo_items.clear();

            for (const auto& [_, item] : _registry)
            {
                _combo_items.push_back({ item.index, (item.*_title_ptr).val() });
            }

            std::sort(_combo_items.begin(), _combo_items.end(), 
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

        std::optional<index_t> render(const char* combo_label, const std::string& current_selection_text, const char* button_text)
        {
            float combo_label_width = ImGui::CalcTextSize(combo_label).x;
            float button_width = ImGui::CalcTextSize(button_text).x + ImGui::GetStyle().FramePadding.x * 2.0f;
            float total_spacing = ImGui::GetStyle().ItemSpacing.x * 3.0f;
            
            float corrected_combo_width = ImGui::GetContentRegionAvail().x - combo_label_width - button_width - total_spacing;
            if (corrected_combo_width < 50.0f) 
            {
                corrected_combo_width = 50.0f;
            }

            std::optional<index_t> result = std::nullopt;

            index_t newly_selected_idx{};
            if (_combo.render(
                combo_label,
                current_selection_text,
                _combo_items,
                newly_selected_idx,
                corrected_combo_width
            ))
            {
                result = newly_selected_idx;
            }

            ImGui::SameLine();
            if (ImGui::Button(button_text))
            {
                result = index_t{}; // Return default-initialized index to signify the "+ New" action
            }

            return result;
        }

    private:
        const TRegistry& _registry;
        corecad::model::property<std::string, model_t> model_t::* _title_ptr;

        using combo_t = searchable_combo<index_t>;
        combo_t _combo;
        std::vector<typename combo_t::item_entry> _combo_items;

    };
}


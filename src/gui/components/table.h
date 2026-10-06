#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include <algorithm>
#include <functional>

#include <imgui.h>

namespace gui::components
{
    struct column_layout_meta
    {
        std::string header_title;
        ImGuiTableColumnFlags flags = ImGuiTableColumnFlags_None;
        float width_or_weight = 0.0f;
    };

    template <typename TCellComponent>
    struct table_column_definition
    {
        column_layout_meta layout;
        TCellComponent cell_widget;
    };

    template <typename TModel, typename... TColumns>
    class table
    {
    public:
        table(
            std::vector<TModel>& data_set,
            std::tuple<table_column_definition<TColumns>...> columns,
            const char* add_row_label = nullptr
        )
            : _data_set { data_set }
            , _columns { std::move(columns) }
            , _add_row_label { add_row_label }
        {
        }

        using value_changed_callback_t = std::function<void(TModel& row_item, size_t column_idx)>;

        void render(
            const char* str_id,
            float target_height,
            value_changed_callback_t on_mutation = nullptr,
            std::function<TModel()> factory_on_add = nullptr
        )
        {
            if (_add_row_label && factory_on_add && ImGui::Button(_add_row_label))
            {
                _data_set.push_back(factory_on_add());
            }

            ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | 
                                          ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;

            constexpr size_t functional_columns_count = sizeof...(TColumns);
            int total_table_columns = static_cast<int>(functional_columns_count) + 1;

            if (ImGui::BeginTable(str_id, total_table_columns, table_flags, ImVec2(0.0f, target_height)))
            {
                std::apply([](const auto&... col) {
                    (ImGui::TableSetupColumn(col.layout.header_title.c_str(), col.layout.flags, col.layout.width_or_weight), ...);
                }, _columns);

                ImGui::TableSetupColumn("Arrangement", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                ImGui::TableHeadersRow();

                size_t index_to_delete = static_cast<size_t>(-1);
                size_t swap_index_a = static_cast<size_t>(-1);
                size_t swap_index_b = static_cast<size_t>(-1);

                for (size_t row_idx = 0; row_idx < _data_set.size(); ++row_idx)
                {
                    ImGui::PushID(static_cast<int>(row_idx));
                    ImGui::TableNextRow();

                    auto& item = _data_set[row_idx];

                    size_t col_idx = 0;
                    std::apply([&](auto&... col) {
                        ([&]() {
                            ImGui::TableSetColumnIndex(static_cast<int>(col_idx));
                            
                            int unique_cell_id = static_cast<int>((row_idx * total_table_columns) + col_idx);
                            ImGui::PushID(unique_cell_id);

                            bool changed = col.cell_widget.render(item);
                            if (on_mutation && changed)
                            {
                                on_mutation(item, col_idx);
                            }

                            ImGui::PopID();
                            col_idx++;
                        }(), ...);
                    }, _columns);

                    ImGui::TableSetColumnIndex(static_cast<int>(functional_columns_count));
                    
                    ImGui::BeginDisabled(row_idx == 0);
                    if (ImGui::Button("^"))
                    {
                        swap_index_a = row_idx;
                        swap_index_b = row_idx - 1;
                    }
                    ImGui::EndDisabled();

                    ImGui::SameLine();
                    ImGui::BeginDisabled(row_idx == _data_set.size() - 1);
                    if (ImGui::Button("v"))
                    {
                        swap_index_a = row_idx;
                        swap_index_b = row_idx + 1;
                    }
                    ImGui::EndDisabled();

                    ImGui::SameLine();
                    if (ImGui::Button("X"))
                    {
                        index_to_delete = row_idx;
                    }

                    ImGui::PopID();
                }

                if (swap_index_a != static_cast<size_t>(-1))
                {
                    std::swap(_data_set[swap_index_a], _data_set[swap_index_b]);
                }
                if (index_to_delete != static_cast<size_t>(-1))
                {
                    _data_set.erase(_data_set.begin() + index_to_delete);
                }

                ImGui::EndTable();
            }
        }

        template <size_t ColIdx>
        auto& get_cell_widget()
        {
            return std::get<ColIdx>(_columns).cell_widget;
        }

    private:
        std::vector<TModel>& _data_set;
        std::tuple<table_column_definition<TColumns>...> _columns;
        const char* _add_row_label;
    };
}

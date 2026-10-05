#include "wall_layers_table.h"

#include "translate.h"

using namespace gui::localization;
using namespace domain::plan::model::shape;
using namespace gui::components;

gui::wall_layers_table::wall_layers_table(context_data ctx)
    : _ctx { ctx }
    , _table {
        _ctx.editing_layers,
        std::make_tuple(
            table_column_definition<static_combo_cell<wall_layer_function, layer_t>> {
                { tr("Function").data(), ImGuiTableColumnFlags_WidthFixed, 130.0f },
                { &layer_t::function, {
                    { tr("Load Bearing").data(), wall_layer_function::load_bearing },
                    { tr("Substrate").data(), wall_layer_function::substrate },
                    { tr("Insulation").data(), wall_layer_function::insulation },
                    { tr("Outer Finish").data(), wall_layer_function::outer_finish },
                    { tr("Inner Finish").data(), wall_layer_function::inner_finish }
                }}
            },
            table_column_definition<registry_combo_cell<layer_t, material_registry_t>> {
                { tr("Material Asset").data(), ImGuiTableColumnFlags_WidthStretch, 0.0f },
                { &layer_t::material, _ctx.materials_lookup, &wall_material_definition::standard_name }
            },
            table_column_definition<length_cell<layer_t>> {
                { tr("Thick (mm)").data(), ImGuiTableColumnFlags_WidthFixed, 100.0f },
                { &layer_t::thickness, 5.0 * corecad::model::mm }
            },
            table_column_definition<integer_cell<layer_t>> {
                { tr("Priority").data(), ImGuiTableColumnFlags_WidthFixed, 100.0f },
                { &layer_t::priority, 10, 0, 1000 }
            },
            table_column_definition<booleans_cell<layer_t>> {
                { tr("BIM Flags").data(), ImGuiTableColumnFlags_WidthFixed, 150.0f },
                { 
                    {
                        {
                            tr("w/ins").data(),
                            &layer_t::wraps_at_inserts,
                            tr("Controls whether layer wraps into the jambs of window and door inserts").data()
                        },
                        {
                            tr("w/ends").data(),
                            &layer_t::wraps_at_ends,
                            tr("Controls whether wall finishes wrap over opening ends").data()
                        }
                    }
                }
            }
        ),
        tr("+ Introduce Layer").data()
    }
{
}

void gui::wall_layers_table::refresh_material_list()
{
    _table.get_cell_widget<1>().refresh_list();
}

void gui::wall_layers_table::render(
    const char* str_id, 
    float layout_height
)
{
    _table.render(
        str_id,
        layout_height,
        [](layer_t& row_item, size_t column_idx) {
            if (column_idx == 0) // The BIM function changed
            {
                // Auto-calculate priority based on BIM classification rules
                switch (row_item.function.val())
                {
                    case wall_layer_function::load_bearing: row_item.priority = 1000; break;
                    case wall_layer_function::substrate: row_item.priority = 600;  break;
                    case wall_layer_function::insulation: row_item.priority = 400;  break;
                    default: row_item.priority = 100;  break;
                }
            }
        },
        []() {
            layer_t default_layer;
            default_layer.thickness = 120.0 * corecad::model::mm;
            default_layer.priority = 500;
            return default_layer;
        }
    );
}

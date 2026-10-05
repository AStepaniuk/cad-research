#pragma once

#include "table.h"
#include "table_cells/integer_cell.h"
#include "table_cells/length_cell.h"
#include "table_cells/booleans_cell.h"
#include "table_cells/static_combo_cell.h"
#include "table_cells/registry_combo_cell.h"

#include "registry.h"
#include "wall_layer.h"
#include "wall_material_definition.h"

namespace gui
{
    class wall_layers_table
    {
    public:
        using layer_t = domain::plan::model::shape::wall_layer;
        using material_registry_t = corecad::model::registry<domain::plan::model::shape::wall_material_definition>;

        using table_t = gui::components::table<layer_t,
            gui::components::static_combo_cell<domain::plan::model::shape::wall_layer_function, layer_t>,
            gui::components::registry_combo_cell<layer_t, material_registry_t>,
            gui::components::length_cell<layer_t>,
            gui::components::integer_cell<layer_t>,
            gui::components::booleans_cell<layer_t>
        >;
 
        struct context_data
        {
            std::vector<layer_t>& editing_layers;
            const material_registry_t& materials_lookup;
        };

        wall_layers_table(context_data ctx);

        void refresh_material_list();

        void render(
            const char* str_id, 
            float layout_height
        );

    private:
        context_data _ctx;

        table_t _table;
    };
}

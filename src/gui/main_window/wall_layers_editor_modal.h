#pragma once

#include <vector>
#include <optional>
#include <imgui.h>

#include "registry.h"
#include "wall_compound_type.h"
#include "wall_layer.h"
#include "wall_material_definition.h"
#include "modal_dialog.h"
#include "registry_combo.h"
#include "static_combo.h"
#include "wall_layers_table.h"

namespace gui
{
    class wall_layers_editor_modal
    {
    public:
        struct context_data
        {
            corecad::model::registry<domain::plan::model::shape::wall_compound_type>& all_compounds;
            corecad::model::registry<domain::plan::model::shape::wall_layer>& all_layers;
            const corecad::model::registry<domain::plan::model::shape::wall_material_definition>& materials_lookup;
        };

        wall_layers_editor_modal(context_data ctx);

        void open(domain::plan::model::shape::wall_compound_type::index_t compound_idx = {});
        
        void process_frame();
        bool is_active() const { return _dialog.is_active(); }

    private:
        void refresh_picker_list();
        void load_compound_into_buffer(domain::plan::model::shape::wall_compound_type::index_t compound_idx);
        void render_layers_table(float table_height);
        void render_compound_metadata_form();
        void render_profile_preview_canvas();
        void save_transaction();

        context_data _ctx;

        domain::plan::model::shape::wall_compound_type _editing_compound;

        components::modal_dialog _dialog;
        components::registry_combo<std::remove_cvref_t<decltype(_ctx.all_compounds)>> _compound_type_picker;
        components::static_combo<domain::plan::model::shape::wall_structural_role> _structural_role_picker;

        std::vector<domain::plan::model::shape::wall_layer> _editing_layers;
        gui::wall_layers_table _layers_table;
    };
}

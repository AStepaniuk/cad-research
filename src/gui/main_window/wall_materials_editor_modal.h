#pragma once

#include <string>
#include <imgui.h>

#include "registry.h"
#include "wall_material_definition.h"
#include "modal_dialog.h"
#include "registry_combo.h"

namespace gui
{
    class wall_materials_editor_modal
    {
    public:
        struct context_data
        {
            corecad::model::registry<domain::plan::model::shape::wall_material_definition>& all_materials;
        };

        wall_materials_editor_modal(context_data ctx);

        void open(domain::plan::model::shape::wall_material_definition::index_t material_idx = {});

        void process_frame();

        bool is_active() const { return _dialog.is_active(); }

    private:
        void refresh_picker_list();
        void load_material_into_buffer(domain::plan::model::shape::wall_material_definition::index_t material_idx);
        void render_material_properties_form();
        void save_transaction();

        context_data _ctx;
        domain::plan::model::shape::wall_material_definition _editing_material;

        components::modal_dialog _dialog;
        components::registry_combo<std::remove_cvref_t<decltype(_ctx.all_materials)>> _material_picker;
    };
}
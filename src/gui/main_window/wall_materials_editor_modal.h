#pragma once

#include <string>
#include <imgui.h>

#include "registry.h"
#include "wall_material_definition.h"
#include "searchable_combo.h"

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

        // Opens the material asset configuration panel for a designated resource index
        void open(domain::plan::model::shape::wall_material_definition::index_t material_idx = {});
        
        // Main processing pass intended for execution inside the primary GUI render tick tree
        void process_frame();

        bool is_active() const { return _is_active; }

    private:
        void refresh_picker_list();
        void load_material_into_buffer(domain::plan::model::shape::wall_material_definition::index_t material_idx);
        void render_material_properties_form();
        void save_transaction();

        context_data _ctx;
        bool _should_open_popup = false;
        bool _is_active = false;

        domain::plan::model::shape::wall_material_definition _editing_material;
        
        // Direct storage character array caches for text input fields
        char _standard_name_buffer[128] = "";
        char _trade_name_buffer[128] = "";
        
        using picker_t = gui::components::searchable_combo<domain::plan::model::shape::wall_material_definition::index_t>;
        picker_t _material_picker;
        std::vector<picker_t::item_entry> _picker_items;
    };
}
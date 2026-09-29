#pragma once

#include <vector>
#include <optional>
#include <imgui.h>

#include "wall_compound_type.h"
#include "wall_layer.h"
#include "wall_material_definition.h"

namespace gui
{
    class wall_layers_editor_modal
    {
    public:
        struct context_data
        {
            std::vector<domain::plan::model::shape::wall_compound_type>* all_compounds;
            std::vector<domain::plan::model::shape::wall_layer>* all_layers;
            const std::vector<domain::plan::model::shape::wall_material_definition>* materials_lookup;
        };

        wall_layers_editor_modal() = default;

        // Opens the layout engine editing panel for a designated compound configuration index
        void open(const context_data& ctx, std::optional<domain::plan::model::shape::wall_compound_type::index_t> compound_idx);
        
        void process_frame();
        bool is_active() const { return _is_active; }

    private:
        void load_compound_into_buffer(std::optional<domain::plan::model::shape::wall_compound_type::index_t> compound_idx);
        void render_layers_table(float table_height);
        void render_compound_metadata_form();
        void render_profile_preview_canvas();
        void save_transaction();

        context_data _ctx{};
        bool _should_open_popup = false;
        bool _is_active = false;

        std::optional<domain::plan::model::shape::wall_compound_type::index_t> _target_compound_idx;
        domain::plan::model::shape::wall_compound_type _editing_compound;
        std::vector<domain::plan::model::shape::wall_layer> _editing_layers;
        
        char _name_buffer[128] = "";
    };
}

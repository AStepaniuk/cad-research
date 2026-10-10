#include "wall_layers_editor_modal.h"

#include <algorithm>
#include <cstring>

#include "translate.h"
#include "form_section.h"
#include "action_strip.h"
#include "input_text.h"

using namespace gui::localization;
using namespace domain::plan::model::shape;
using namespace corecad::model;

gui::wall_layers_editor_modal::wall_layers_editor_modal(context_data ctx)
    : _ctx { ctx }
    , _dialog { ImVec2(850.0f, 600.0f), ImVec2(650.0f, 510.0f), ImVec2(1200.0f, 900.0f) }
    , _compound_type_picker { _ctx.all_compounds, &wall_compound_type::name }
    , _structural_role_picker {{
        { wall_structural_role::partition_wall, tr("Partition Wall").data() },
        { wall_structural_role::bearing_interior, tr("Bearing Interior").data() },
        { wall_structural_role::bearing_exterior, tr("Bearing Exterior").data() },
        { wall_structural_role::shear_wall, tr("Shear Wall").data() }
    }}
    , _layers_table {{ _editing_layers, _ctx.materials_lookup }}
    , _profile_preview_canvas {{ _editing_layers, _ctx.materials_lookup }}
{
}

void gui::wall_layers_editor_modal::open(wall_compound_type::index_t compound_idx)
{
    _dialog.open();
    
    load_compound_into_buffer(compound_idx);
    refresh_picker_list();
    _layers_table.refresh_material_list();
}

void gui::wall_layers_editor_modal::refresh_picker_list()
{
    _compound_type_picker.refresh();
}

void gui::wall_layers_editor_modal::load_compound_into_buffer(wall_compound_type::index_t compound_idx)
{
    _editing_layers.clear();

    if (compound_idx)
    {
        _editing_compound = _ctx.all_compounds.get(compound_idx);

        for (const auto& layer_idx : _editing_compound.layers)
        {
            _editing_layers.push_back(_ctx.all_layers.get(layer_idx));
        }
    }
    else
    {
        _editing_compound = wall_compound_type();
        _editing_compound.name = std::string { tr("New Compound Type").data() };
    }
}

void gui::wall_layers_editor_modal::process_frame()
{
    if (const auto h = _dialog.begin(tr("Compound Layer Hierarchy Editor").data()))
    {
        render_compound_metadata_form();        
        render_editor_workspace_layout();
        
        int action = components::action_strip({
            components::button_meta { .title = tr("Save Structural Changes").data(), .is_default = true },
            components::button_meta { .title = tr("Discard Modifications").data() }
        });

        if (action == 0)
        {
            save_transaction();
            _dialog.close();
        }
        else if (action == 1)
        {
            _dialog.close();
        }
    }
}

void gui::wall_layers_editor_modal::render_compound_metadata_form()
{
    std::string selection_label = tr("<New Compound Profile>").data();
    if (_editing_compound.index)
    {
        selection_label = _editing_compound.name.val();
    }

    if (const auto idx = _compound_type_picker.render(tr("Active Compound").data(), selection_label, tr("+ New Compound").data()); idx)
    {
        load_compound_into_buffer(idx.value());
    }

    ImGui::Separator();
    ImGui::Spacing();

    components::input_text(tr("Profile Designation:").data(), _editing_compound.name);
    _structural_role_picker.render(tr("Structural Classification:").data(), _editing_compound.structural_role);
    ImGui::Spacing();
}

void gui::wall_layers_editor_modal::render_editor_workspace_layout()
{
    ImVec2 available_space = ImGui::GetContentRegionAvail();
    
    float dynamic_workspace_height = available_space.y - 140.0f;

    float canvas_row_height = dynamic_workspace_height * 0.45f;
    float table_row_height = dynamic_workspace_height - canvas_row_height;

    if (table_row_height < 140.0f) table_row_height = 140.0f;
    if (canvas_row_height < 100.0f) canvas_row_height = 100.0f;

    components::form_section(tr("Structural Composition Strategy (Ordered Outside to Inside)").data());
    
    _layers_table.render("##BIMWallLayersGrid", table_row_height);

    ImGui::Spacing();

    components::form_section(tr("Structural Preview").data());
    
   _profile_preview_canvas.render("##BIMProfileVectorPreviewCanvas", available_space.x, canvas_row_height);
    
    ImGui::Spacing();
}

void gui::wall_layers_editor_modal::save_transaction()
{
    _editing_compound.layers.clear();
    for (auto& dynamic_layer : _editing_layers)
    {
        if (dynamic_layer.index)
        {
            _ctx.all_layers.get(dynamic_layer.index) = dynamic_layer;
        }
        else
        {
            dynamic_layer.index = _ctx.all_layers.put(dynamic_layer);
        }
        _editing_compound.layers.put(dynamic_layer.index);
    }

    if (_editing_compound.index)
    {
        _ctx.all_compounds.get(_editing_compound.index) = _editing_compound;
    }
    else
    {
        _editing_compound.index = _ctx.all_compounds.put(_editing_compound);
    }
}

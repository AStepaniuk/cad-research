#include "wall_materials_editor_modal.h"

#include <algorithm>
#include <cstring>

#include "translate.h"
#include "modal_dialog.h"
#include "input_scalar_clamped.h"
#include "input_text.h"
#include "action_strip.h"
#include "form_section.h"

using namespace gui::localization;
using namespace domain::plan::model::shape;

void gui::wall_materials_editor_modal::open(wall_material_definition::index_t material_idx)
{
    _dialog.open();
    
    load_material_into_buffer(material_idx);
    refresh_picker_list();
}

void gui::wall_materials_editor_modal::refresh_picker_list()
{
    _material_picker.refresh();
}

void gui::wall_materials_editor_modal::load_material_into_buffer(wall_material_definition::index_t material_idx)
{
    if (material_idx)
    {
        _editing_material = _ctx.all_materials.get(material_idx);
    }
    else
    {
        _editing_material = wall_material_definition();
        
        _editing_material.standard_name = std::string { tr("New Structural Material") };
        _editing_material.trade_name = std::string { tr("Generic Specifications") };
        _editing_material.thermal_conductivity = 1.0;
        _editing_material.density = 1000.0;
    }
}

gui::wall_materials_editor_modal::wall_materials_editor_modal(context_data ctx)
    : _ctx { ctx }
    , _dialog { ImVec2(600.0f, 320.0f), ImVec2(550.0f, 100.0f), ImVec2(700.0f, 400.0f) }
    , _material_picker { _ctx.all_materials, &wall_material_definition::standard_name }
{
}

void gui::wall_materials_editor_modal::process_frame()
{
    if (const auto h = _dialog.begin(tr("BIM Material Resource Manager").data(), ImGuiWindowFlags_AlwaysAutoResize); h)
    {
        render_material_properties_form();

        auto action = components::action_strip({
            components::button_meta { .title = tr("Save Material Asset").data(), .is_default = true },
            components::button_meta { .title = tr("Close").data() }
        });

        if (action == 0)
        {
            save_transaction();
            load_material_into_buffer(_editing_material.index);
            refresh_picker_list();
        }
        else if (action == 1)
        {
            _dialog.close();
        }
    }
}

void gui::wall_materials_editor_modal::render_material_properties_form()
{
    std::string selection_label = tr("<New Material Profile>").data();
    if (_editing_material.index)
    {
        selection_label = _editing_material.standard_name.val();
    }

    if (const auto idx = _material_picker.render(tr("Active Material").data(), selection_label, tr("+ New Material").data()); idx)
    {
        load_material_into_buffer(idx.value());
    }

    components::form_section(tr("Semantic Nomenclature & Trade Identifiers").data());
    
    components::input_text(tr("Classification Name (IFC):").data(), _editing_material.standard_name);
    components::input_text(tr("Commercial Trade Name / Code:").data(), _editing_material.trade_name);

    components::form_section(tr("BIM Engineering Physics & Thermal Performance Metrics").data());

    components::input_scalar_clamped(
        tr("Thermal Conductivity:").data(),
        _editing_material.thermal_conductivity,
        0.001, 50.0,
        tr("%.4f W/(m·K)").data(),
        0.001
    );

    components::input_scalar_clamped(
        tr("Volumetric Density:").data(),
        _editing_material.density,
        1.0, 10000.0,
        tr("%.1f kg/m³").data(),
        10.0
    );
}

void gui::wall_materials_editor_modal::save_transaction()
{
    if (_editing_material.index)
    {
        _ctx.all_materials.get(_editing_material.index) = _editing_material;
    }
    else
    {
        _editing_material.index = _ctx.all_materials.put(_editing_material);
    }
}

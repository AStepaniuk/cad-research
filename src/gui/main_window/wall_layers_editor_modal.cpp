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
    , _dialog { ImVec2(850.0f, 600.0f), ImVec2(650.0f, 300.0f), ImVec2(1200.0f, 900.0f) }
    , _compound_type_picker { _ctx.all_compounds, &wall_compound_type::name }
    , _structural_role_picker {{
        { wall_structural_role::partition_wall, tr("Partition Wall").data() },
        { wall_structural_role::bearing_interior, tr("Bearing Interior").data() },
        { wall_structural_role::bearing_exterior, tr("Bearing Exterior").data() },
        { wall_structural_role::shear_wall, tr("Shear Wall").data() }
    }}
    , _layers_table {{ _editing_layers, _ctx.materials_lookup }}
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
        
        components::form_section(tr("Structural Composition Strategy (Ordered Outside to Inside)").data());

        size_t row_count = _editing_layers.size();
        float row_height = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().CellPadding.y * 2.0f;
        float header_height = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().CellPadding.y * 2.0f;
        
        float dynamic_table_height = 0.0f;
        if (row_count == 0)
        {
            dynamic_table_height = 100.0f;
        }
        else
        {
            float desired_h = header_height + (static_cast<float>(row_count) * row_height) + 10.0f;
            dynamic_table_height = std::clamp(desired_h, 120.0f, 350.0f);
        }

        render_layers_table(dynamic_table_height);
        
        components::form_section(tr("Structural Preview").data());
        render_profile_preview_canvas();
        
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

    _structural_role_picker.render(tr("Structural Classification").data(), _editing_compound.structural_role);

    ImGui::Spacing();
}

void gui::wall_layers_editor_modal::render_layers_table(float table_height)
{
    _layers_table.render("##LayersBIMTable_Old", table_height);
}

void gui::wall_layers_editor_modal::render_profile_preview_canvas()
{
    float canvas_height = 120.0f;
    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImVec2(ImGui::GetContentRegionAvail().x, canvas_height);

    if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_FrameBg);
    ImU32 border_color = ImGui::GetColorU32(ImGuiCol_Border);
    draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), bg_color, 4.0f);
    draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), border_color, 4.0f);

    // Prevent rendering sweeps if there are zero layers defined in the buffer workspace
    if (_editing_layers.empty())
    {
        std::string_view stub_label { tr("Define wall layers to populate preview structure") };
        ImVec2 text_size = ImGui::CalcTextSize(stub_label.data());
        ImVec2 text_pos = ImVec2(canvas_pos.x + (canvas_size.x - text_size.x) * 0.5f, canvas_pos.y + (canvas_size.y - text_size.y) * 0.5f);
        draw_list->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_TextDisabled), stub_label.data());
        ImGui::Dummy(canvas_size);
        return;
    }

    double total_thickness_mm = 0.0;
    for (const auto& layer : _editing_layers)
    {
        total_thickness_mm += layer.thickness.val().numerical_value_in(mm);
    }
    if (total_thickness_mm <= 0.0) total_thickness_mm = 1.0;

    // Establish scale boundaries leaving margin paddings on the edges
    float margin_y = 20.0f;
    float margin_x = 40.0f;
    float drawable_height = canvas_size.y - (margin_y * 2.0f);
    float drawable_width = canvas_size.x - (margin_x * 2.0f);

    // Compute dynamic scaling coefficient (pixels per millimeter)
    float scale = drawable_height / static_cast<float>(total_thickness_mm);

    // Establish spatial anchoring centerplanes
    float current_y = canvas_pos.y + margin_y;
    float start_x = canvas_pos.x + margin_x;
    float end_x = canvas_pos.x + canvas_size.x - margin_x;

    // Track structural core boundaries to calculate BIM wrapping end-caps accurately
    float core_top_y = current_y;
    float core_bottom_y = current_y + (static_cast<float>(total_thickness_mm) * scale);

    // Find the primary load-bearing core boundaries to snap wrapping offsets cleanly
    float current_scan_y = current_y;
    for (const auto& layer : _editing_layers)
    {
        float layer_h = static_cast<float>(layer.thickness.val().numerical_value_in(mm)) * scale;
        if (layer.function.val() == wall_layer_function::load_bearing)
        {
            core_top_y = current_scan_y;
            core_bottom_y = current_scan_y + layer_h;
            break; 
        }
        current_scan_y += layer_h;
    }

    // 3. Render Individual BIM Structural Strata Layers sequentially
    for (size_t i = 0; i < _editing_layers.size(); ++i)
    {
        const auto& layer = _editing_layers[i];
        float layer_h = static_cast<float>(layer.thickness.val().numerical_value_in(mm)) * scale;

        // Determine procedural fills/colors based on BIM architectural functions
        ImU32 layer_fill = ImGui::GetColorU32(ImVec4(0.35f, 0.35f, 0.35f, 1.0f)); // Substrate default
        ImU32 layer_stroke = ImGui::GetColorU32(ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

        switch (layer.function.val())
        {
            case wall_layer_function::load_bearing:
                layer_fill = ImGui::GetColorU32(ImVec4(0.2f, 0.4f, 0.6f, 0.6f));   // Deep Blue Structural Core
                layer_stroke = ImGui::GetColorU32(ImVec4(0.3f, 0.5f, 0.8f, 1.0f));
                break;
            case wall_layer_function::insulation:
                layer_fill = ImGui::GetColorU32(ImVec4(0.7f, 0.6f, 0.2f, 0.5f));   // Amber/Yellow Insulation Fill
                layer_stroke = ImGui::GetColorU32(ImVec4(0.8f, 0.7f, 0.3f, 1.0f));
                break;
            case wall_layer_function::outer_finish:
                layer_fill = ImGui::GetColorU32(ImVec4(0.5f, 0.2f, 0.2f, 0.6f));   // Terracotta Outer Finish
                layer_stroke = ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 1.0f));
                break;
            case wall_layer_function::inner_finish:
                layer_fill = ImGui::GetColorU32(ImVec4(0.6f, 0.6f, 0.6f, 0.4f));   // Light Gray Interior Gypsum Board
                layer_stroke = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
                break;
            default: break;
        }

        // --- BIM WRAPPING RENDER RULES ENGINE ---
        if (layer.wraps_at_ends.val())
        {
            // Calculate a horizontal extension offset shift matching layer stack sequence indices
            float lateral_wrap_extension = 6.0f + (static_cast<float>(i) * 3.0f);

            // Extend layer profile endpoints outward to completely enclose underlying layers
            float wrapped_start_x = start_x - lateral_wrap_extension;
            float wrapped_end_x = end_x + lateral_wrap_extension;

            // Generate structural capping geometry shapes enclosing the core bounds
            draw_list->AddRectFilled(ImVec2(wrapped_start_x, current_y), ImVec2(wrapped_end_x, current_y + layer_h), layer_fill);
            
            // Draw matching vertical bounding caps to simulate material wrap-around returns
            draw_list->AddRectFilled(ImVec2(wrapped_start_x, current_y), ImVec2(start_x, core_bottom_y), layer_fill);
            draw_list->AddRectFilled(ImVec2(end_x, current_y), ImVec2(wrapped_end_x, core_bottom_y), layer_fill);

            // Draw line strokes to clearly denote material continuity wraps
            draw_list->AddLine(ImVec2(wrapped_start_x, current_y), ImVec2(wrapped_end_x, current_y), layer_stroke, 1.5f);
            draw_list->AddLine(ImVec2(wrapped_start_x, current_y), ImVec2(wrapped_start_x, core_bottom_y), layer_stroke, 1.5f);
            draw_list->AddLine(ImVec2(wrapped_end_x, current_y), ImVec2(wrapped_end_x, core_bottom_y), layer_stroke, 1.5f);
        }
        else
        {
            // Standard Unwrapped rendering pass: clean rectangular layout band
            draw_list->AddRectFilled(ImVec2(start_x, current_y), ImVec2(end_x, current_y + layer_h), layer_fill);
            draw_list->AddRect(ImVec2(start_x, current_y), ImVec2(end_x, current_y + layer_h), layer_stroke, 1.0f);
        }

        // Programmatic Inserts Indicator Check (`wraps_at_inserts`)
        if (layer.wraps_at_inserts.val())
        {
            // Draw a subtle centerline hash marker to denote custom geometry intersection points
            float mid_x = start_x + (end_x - start_x) * 0.5f;
            draw_list->AddLine(ImVec2(mid_x - 10.0f, current_y + layer_h * 0.5f), 
                               ImVec2(mid_x + 10.0f, current_y + layer_h * 0.5f), 
                               ImGui::GetColorU32(ImVec4(0.2f, 1.0f, 0.2f, 0.8f)), 1.5f);
        }

        current_y += layer_h; // Increment spatial projection plane downwards
    }

    ImGui::Dummy(canvas_size); // Explicitly reserve widget layout space inside the parent window
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

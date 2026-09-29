#include "wall_layers_editor_modal.h"
#include <algorithm>
#include <cstring>
#include "translate.h"

using namespace gui::localization;
using namespace domain::plan::model::shape;
using namespace corecad::model;

void gui::wall_layers_editor_modal::open(const context_data& ctx, std::optional<wall_compound_type::index_t> compound_idx)
{
    _ctx = ctx;
    _should_open_popup = true;
    _is_active = true;
    
    load_compound_into_buffer(compound_idx);
}

void gui::wall_layers_editor_modal::load_compound_into_buffer(std::optional<wall_compound_type::index_t> compound_idx)
{
    _target_compound_idx = compound_idx;
    _editing_layers.clear();

    if (_target_compound_idx && _ctx.all_compounds)
    {
        auto it = std::find_if(_ctx.all_compounds->begin(), _ctx.all_compounds->end(),
            [this](const auto& comp) { return comp.index == *_target_compound_idx; });

        if (it != _ctx.all_compounds->end())
        {
            _editing_compound = *it;
            std::strncpy(_name_buffer, _editing_compound.name.val().c_str(), sizeof(_name_buffer) - 1);

            if (_ctx.all_layers)
            {
                for (const auto& layer_idx : _editing_compound.layers)
                {
                    auto layer_it = std::find_if(_ctx.all_layers->begin(), _ctx.all_layers->end(),
                        [&layer_idx](const auto& lay) { return lay.index == layer_idx; });
                    
                    if (layer_it != _ctx.all_layers->end())
                    {
                        _editing_layers.push_back(*layer_it);
                    }
                }
            }
            return;
        }
    }

    _editing_compound = wall_compound_type();
    std::strncpy(_name_buffer, tr("New Compound Type").data(), sizeof(_name_buffer) - 1);
}

void gui::wall_layers_editor_modal::process_frame()
{
    if (!_is_active) return;

    if (_should_open_popup)
    {
        ImGui::OpenPopup(tr("Compound Layer Hierarchy Editor").data());
        _should_open_popup = false;
    }

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    if (main_viewport)
    {
        // Find the absolute midpoint of the current primary application footprint
        ImVec2 viewport_center = ImVec2(
            main_viewport->Pos.x + main_viewport->Size.x * 0.5f,
            main_viewport->Pos.y + main_viewport->Size.y * 0.5f
        );

        // Center pivot alignment anchor: (0.5f, 0.5f) explicitly sets the window's center as the alignment handle
        ImGui::SetNextWindowPos(viewport_center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    }

    ImVec2 target_default_size = ImVec2(750.0f, 600.0f);
    if (main_viewport)
    {
        if (target_default_size.x > main_viewport->Size.x - 40.0f) target_default_size.x = main_viewport->Size.x - 40.0f;
        if (target_default_size.y > main_viewport->Size.y - 40.0f) target_default_size.y = main_viewport->Size.y - 40.0f;
    }
    ImGui::SetNextWindowSize(target_default_size, ImGuiCond_FirstUseEver);
    
    if (ImGui::BeginPopupModal(tr("Compound Layer Hierarchy Editor").data(), &_is_active, ImGuiWindowFlags_AlwaysAutoResize))
    {
        render_compound_metadata_form();
        
        ImGui::TextDisabled("%s", tr("Structural Composition Strategy (Ordered Outside to Inside)").data());
        ImGui::Separator();

        // Calculate exact remaining heights needed for bottom elements:
        // 120.0f (Canvas) + Text headers (~20.0f) + Buttons (~35.0f) + Separators/Spacings (~35.0f)
        float reserved_bottom_height = 120.0f + 20.0f + 35.0f + 35.0f;
        float dynamic_table_height = ImGui::GetContentRegionAvail().y - reserved_bottom_height;

        // Establish a healthy minimum boundary constraint so the table never disappears 
        if (dynamic_table_height < 150.0f) dynamic_table_height = 150.0f;

        // Pass the dynamically scaled height constraint straight down to our table renderer
        render_layers_table(dynamic_table_height);

        ImGui::Separator();
        
        ImGui::TextDisabled("%s", tr("Structural Preview").data());
        render_profile_preview_canvas(); // <-- New structural drawing call pass
        
        ImGui::Separator();
        
        if (ImGui::Button(tr("Commit Structural Changes").data(), ImVec2(180, 0)))
        {
            save_transaction();
            ImGui::CloseCurrentPopup();
            _is_active = false;
        }
        ImGui::SameLine();
        if (ImGui::Button(tr("Discard Modifications").data(), ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            _is_active = false;
        }

        ImGui::EndPopup();
    }
}

void gui::wall_layers_editor_modal::render_compound_metadata_form()
{
    std::string current_selection_label { tr("<New Compound Profile>") };
    if (_target_compound_idx && _ctx.all_compounds)
    {
        current_selection_label = _editing_compound.name.val();
    }

    // Explicitly measure all individual horizontal layout elements
    std::string combo_label_text { tr("Target Compound") };
    std::string button_text { tr("+ New Profile") };

    float combo_label_width = ImGui::CalcTextSize(combo_label_text.c_str()).x;
    float button_width = ImGui::CalcTextSize(button_text.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float total_spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f; // Padding between: [ComboBox] <sp> [Label] <sp> [Button]

    // Calculate the precise box width so the trailing elements fit perfectly within the window bounds
    float corrected_box_width = ImGui::GetContentRegionAvail().x - combo_label_width - button_width - total_spacing;

    // Guard against unusually narrow windows
    if (corrected_box_width < 50.0f) corrected_box_width = 50.0f;

    ImGui::SetNextItemWidth(corrected_box_width);
    if (ImGui::BeginCombo(tr("Target Compound").data(), current_selection_label.c_str()))
    {
        if (_ctx.all_compounds)
        {
            for (const auto& comp : *_ctx.all_compounds)
            {
                bool is_selected = (_target_compound_idx && *_target_compound_idx == comp.index);
                if (ImGui::Selectable(comp.name.val().c_str(), is_selected))
                {
                    // Swap working copy buffer targets cleanly on a click selection pass
                    load_compound_into_buffer(comp.index);
                }
            }
        }
        ImGui::EndCombo();
    }
    
    ImGui::SameLine();
    
    if (ImGui::Button(tr("+ New Profile").data()))
    {
        // Flushes buffers back to blank base properties initialization slate
        load_compound_into_buffer(std::nullopt);
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", tr("Clear editing buffer workspace to define a blank compound configuration model").data());
    }

    ImGui::Separator();
    ImGui::Spacing();

if (ImGui::InputText(tr("Profile Designation").data(), _name_buffer, sizeof(_name_buffer)))
    {
        _editing_compound.name = _name_buffer;
    }

    int role_int = static_cast<int>(_editing_compound.structural_role.val());
    const char* roles[] = { "Partition Wall", "Bearing Interior", "Bearing Exterior", "Shear Wall" };
    int selected_idx = 0;

    if (role_int == 600) selected_idx = 1;
    else if (role_int == 800) selected_idx = 2;
    else if (role_int == 1000) selected_idx = 3;

    if (ImGui::Combo(tr("Structural Classification").data(), &selected_idx, roles, IM_ARRAYSIZE(roles)))
    {
        wall_structural_role updated_role = wall_structural_role::partition_wall;
        if (selected_idx == 1) updated_role = wall_structural_role::bearing_interior;
        else if (selected_idx == 2) updated_role = wall_structural_role::bearing_exterior;
        else if (selected_idx == 3) updated_role = wall_structural_role::shear_wall;
        
        _editing_compound.structural_role = updated_role;
    }
    ImGui::Spacing();
}

void gui::wall_layers_editor_modal::render_layers_table(float table_height)
{
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
    
    if (ImGui::Button(tr("+ Introduce Layer").data()))
    {
        wall_layer default_layer;
        default_layer.thickness = 120.0 * mm;
        _editing_layers.push_back(default_layer);
    }
    
    if (ImGui::BeginTable("##LayersBIMTable", 6, flags, ImVec2(0.0f, table_height)))
    {
        ImGui::TableSetupColumn(tr("Function").data(), ImGuiTableColumnFlags_WidthFixed, 130.0f);
        ImGui::TableSetupColumn(tr("Material Asset").data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(tr("Thick (mm)").data(), ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn(tr("Priority").data(), ImGuiTableColumnFlags_WidthFixed, 70.0f);
        ImGui::TableSetupColumn(tr("BIM Flags").data(), ImGuiTableColumnFlags_WidthFixed, 110.0f);
        ImGui::TableSetupColumn(tr("Arrangement").data(), ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();

        size_t index_to_delete = static_cast<size_t>(-1);
        
        for (size_t i = 0; i < _editing_layers.size(); ++i)
        {
            auto& layer = _editing_layers[i];
            ImGui::PushID(static_cast<int>(i));

            ImGui::TableNextRow();
            
            // Column 0: BIM Function
            ImGui::TableSetColumnIndex(0);
            int func_idx = static_cast<int>(layer.function.val());
            const char* functions[] = { "Load Bearing", "Substrate", "Insulation", "Outer Finish", "Inner Finish" };
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::Combo("##Func", &func_idx, functions, IM_ARRAYSIZE(functions)))
            {
                layer.function = static_cast<wall_layer_function>(func_idx);
                if (func_idx == 0) layer.priority = 1000;
                else if (func_idx == 1) layer.priority = 600;
                else if (func_idx == 2) layer.priority = 400;
                else layer.priority = 100;
            }

            // Column 1: Material Mapping
            ImGui::TableSetColumnIndex(1);
            std::string material_preview = tr("<Unassigned>").data();
            if (_ctx.materials_lookup)
            {
                for (const auto& mat : *_ctx.materials_lookup)
                {
                    if (mat.index == layer.material)
                    {
                        material_preview = mat.standard_name;
                        break;
                    }
                }
            }
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::BeginCombo("##Material", material_preview.c_str()))
            {
                if (_ctx.materials_lookup)
                {
                    for (const auto& mat : *_ctx.materials_lookup)
                    {
                        bool is_selected = (mat.index == layer.material.val());
                        if (ImGui::Selectable(mat.standard_name.val().c_str(), is_selected))
                        {
                            layer.material = mat.index;
                        }
                    }
                }
                ImGui::EndCombo();
            }

            // Column 2: Thickness
            ImGui::TableSetColumnIndex(2);
            double thk = layer.thickness.val().numerical_value_in(mm);
            double min_thk = 0.0;
            double max_thk = 1000.0;
            float speed = 0.5f;
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::DragScalar("##Thick", ImGuiDataType_Double, &thk, speed, &min_thk, &max_thk, "%.1f"))
            {
                layer.thickness = thk * mm;
            }
            // Column 3: Priority
            ImGui::TableSetColumnIndex(3);
            int priority = layer.priority;
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::DragInt("##Prio", &priority, 5.0f, 0, 1000))
            {
                layer.priority = priority;
            }

            // Column 4: Architectural Wrapping Rules
            ImGui::TableSetColumnIndex(4);
            bool wraps_e = layer.wraps_at_ends;
            bool wraps_i = layer.wraps_at_inserts;
            ImGui::Checkbox("E", &wraps_e); 
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", tr("Wraps at boundary edge ends").data());
            ImGui::SameLine();
            ImGui::Checkbox("I", &wraps_i);
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("%s", tr("Wraps at programmatic inserts (Windows/Doors)").data());
            
            layer.wraps_at_ends = wraps_e;
            layer.wraps_at_inserts = wraps_i;

            // Column 5: Positional Rearrangement Matrix
            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button("^") && i > 0)
            {
                std::swap(_editing_layers[i], _editing_layers[i - 1]);
            }
            ImGui::SameLine();
            if (ImGui::Button("v") && i < _editing_layers.size() - 1)
            {
                std::swap(_editing_layers[i], _editing_layers[i + 1]);
            }
            ImGui::SameLine();
            if (ImGui::Button("X"))
            {
                index_to_delete = i;
            }

            ImGui::PopID();
        }

        if (index_to_delete != static_cast<size_t>(-1))
        {
            _editing_layers.erase(_editing_layers.begin() + index_to_delete);
        }

        ImGui::EndTable();
    }
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
    if (!_ctx.all_compounds || !_ctx.all_layers) return;

    _editing_compound.layers.clear();
    for (auto& dynamic_layer : _editing_layers)
    {
        if (!dynamic_layer.index)
        {
            static wall_layer::index_t incremental_layer_id_mock { 50000 };
            dynamic_layer.index = ++incremental_layer_id_mock;
            _ctx.all_layers->push_back(dynamic_layer);
        }
        else
        {
            for (auto& active_lay : *_ctx.all_layers)
            {
                if (active_lay.index == dynamic_layer.index)
                {
                    active_lay = dynamic_layer;
                    break;
                }
            }
        }
        _editing_compound.layers.put(dynamic_layer.index);
    }

    if (_target_compound_idx)
    {
        for (auto& comp : *_ctx.all_compounds)
        {
            if (comp.index == *_target_compound_idx)
            {
                comp = _editing_compound;
                break;
            }
        }
    }
    else
    {
        static wall_compound_type::index_t incremental_comp_id_mock { 20000 };
        _editing_compound.index = ++incremental_comp_id_mock;
        _ctx.all_compounds->push_back(_editing_compound);
    }
}

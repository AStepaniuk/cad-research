#include "wall_materials_editor_modal.h"
#include <algorithm>
#include <cstring>
#include "translate.h"

using namespace gui::localization;
using namespace domain::plan::model::shape;

namespace
{
    bool contains_case_insensitive(const std::string& haystack, const std::string& needle)
    {
        if (needle.empty()) return true;
        auto it = std::search(
            haystack.begin(), haystack.end(),
            needle.begin(), needle.end(),
            [](char ch1, char ch2) { return std::tolower(ch1) == std::tolower(ch2); }
        );
        return it != haystack.end();
    }
}

void gui::wall_materials_editor_modal::open(wall_material_definition::index_t material_idx)
{
    _should_open_popup = true;
    _is_active = true;
    
    // Delegate initial data parsing sweep
    load_material_into_buffer(material_idx);
    refresh_picker_list();
}

void gui::wall_materials_editor_modal::refresh_picker_list()
{
    _picker_items.clear();

    for (const auto& [_, mat] : _ctx.all_materials)
    {
        _picker_items.push_back({ mat.index, mat.standard_name.val() });
    }

    std::sort(_picker_items.begin(), _picker_items.end(), 
        [](const auto& a, const auto& b) {
            return std::lexicographical_compare(
                a.display_name.begin(), a.display_name.end(),
                b.display_name.begin(), b.display_name.end(),
                [](char ch1, char ch2) {
                    return std::tolower(static_cast<unsigned char>(ch1)) < 
                           std::tolower(static_cast<unsigned char>(ch2));
                }
            );
        }
    );
}

void gui::wall_materials_editor_modal::load_material_into_buffer(wall_material_definition::index_t material_idx)
{
    if (material_idx)
    {
        _editing_material = _ctx.all_materials.get(material_idx);

            std::strncpy(_standard_name_buffer, _editing_material.standard_name.val().c_str(), sizeof(_standard_name_buffer) - 1);
            std::strncpy(_trade_name_buffer, _editing_material.trade_name.val().c_str(), sizeof(_trade_name_buffer) - 1);
    }
    else
    {
        // Default BIM compliant fallback workspace initialization state (+ New Material trigger pass)
        _editing_material = wall_material_definition();
        std::strncpy(_standard_name_buffer, tr("New Structural Material").data(), sizeof(_standard_name_buffer) - 1);
        std::strncpy(_trade_name_buffer, tr("Generic Specifications").data(), sizeof(_trade_name_buffer) - 1);
        
        _editing_material.standard_name = _standard_name_buffer;
        _editing_material.trade_name = _trade_name_buffer;
        _editing_material.thermal_conductivity = 1.0; // Standard nominal unit fallback defaults
        _editing_material.density = 1000.0;
    }
}

gui::wall_materials_editor_modal::wall_materials_editor_modal(context_data ctx)
    : _ctx { ctx }
{
}

void gui::wall_materials_editor_modal::process_frame()
{
    if (!_is_active)
    {
        return;
    }

    if (_should_open_popup)
    {
        ImGui::OpenPopup(tr("BIM Material Resource Manager").data());
        _should_open_popup = false;
    }

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 min_size = ImVec2(550.0f, 100.0f); // Low floor lets window wrap tightly around sparse data
    ImVec2 max_size = ImVec2(700.0f, 400.0f); // Lowered ceiling caps maximum expansion heights
    
    if (main_viewport)
    {
        if (max_size.x > main_viewport->Size.x - 40.0f) max_size.x = main_viewport->Size.x - 40.0f;
        if (max_size.y > main_viewport->Size.y - 40.0f) max_size.y = main_viewport->Size.y - 40.0f;
    }

    ImGui::SetNextWindowSizeConstraints(min_size, max_size);
    ImGui::SetNextWindowSize(ImVec2(600.0f, 320.0f), ImGuiCond_FirstUseEver); // Lean base profile 
    
    // AlwaysAutoResize recalculates height on the fly and forces a tight content wrap
    if (ImGui::BeginPopupModal(tr("BIM Material Resource Manager").data(), &_is_active, ImGuiWindowFlags_AlwaysAutoResize))
    {
        render_material_properties_form();

        ImGui::Separator();
        ImGui::Spacing();
        
        bool enter_pressed = ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter);

        if (ImGui::Button(tr("Save Material Asset").data(), ImVec2(160, 0)) || enter_pressed)
        {
            save_transaction();
            load_material_into_buffer(_editing_material.index);
            refresh_picker_list();
        }
        ImGui::SameLine();
        if (ImGui::Button(tr("Close").data(), ImVec2(100, 0)))
        {
            ImGui::CloseCurrentPopup();
            _is_active = false;
        }

        ImGui::EndPopup();
    }
}

void gui::wall_materials_editor_modal::render_material_properties_form()
{
    std::string current_selection_label = tr("<New Material Profile>").data();
    if (_editing_material.index)
    {
        current_selection_label = _editing_material.standard_name.val();
    }

    std::string combo_label_text = tr("Active Material").data();
    std::string button_text = tr("+ New Material").data();

    float combo_label_width = ImGui::CalcTextSize(combo_label_text.c_str()).x;
    float button_width = ImGui::CalcTextSize(button_text.c_str()).x + ImGui::GetStyle().FramePadding.x * 2.0f;
    float total_spacing = ImGui::GetStyle().ItemSpacing.x * 3.0f;
    float corrected_combo_width = ImGui::GetContentRegionAvail().x - combo_label_width - button_width - total_spacing;
    
    if (corrected_combo_width < 50.0f) corrected_combo_width = 50.0f;

    domain::plan::model::shape::wall_material_definition::index_t newly_selected_idx;
    if (_material_picker.render(
        tr("Active Material").data(),
        current_selection_label,
        _picker_items,
        newly_selected_idx,
        corrected_combo_width
    ))
    {
        load_material_into_buffer(newly_selected_idx);
    }

    ImGui::SameLine();
    if (ImGui::Button(tr("+ New Material").data()))
    {
        load_material_into_buffer({});
    }
    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", tr("Initialize a clean Open BIM baseline specification matrix entry").data());
    }

    ImGui::Separator();
    ImGui::Spacing();

    // --- 2. MULTI-COLUMN DETAILED PHYSICAL ATTRIBUTES MATRIX ---
    ImGui::TextDisabled("%s", tr("Semantic Nomenclature & Trade Identifiers").data());
    
    if (ImGui::InputText(tr("Standard Classification Name (IFC)").data(), _standard_name_buffer, sizeof(_standard_name_buffer)))
    {
        _editing_material.standard_name = _standard_name_buffer;
    }
    if (ImGui::InputText(tr("Commercial Trade Name / Code").data(), _trade_name_buffer, sizeof(_trade_name_buffer)))
    {
        _editing_material.trade_name = _trade_name_buffer;
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::TextDisabled("%s", tr("BIM Engineering Physics & Thermal Performance Metrics").data());

    // Variable property extraction leveraging native double scalar precision
    double conductivity = _editing_material.thermal_conductivity.val();
    double density = _editing_material.density.val();

    double min_cond = 0.001;
    double max_cond = 50.0;
    double min_dens = 1.0;
    double max_dens = 10000.0;
    double cond_step = 0.001; 
    double dens_step = 10.0;

    ImGui::TextUnformatted(tr("Thermal Conductivity:").data());
    ImGui::SameLine(220.0f); 
    ImGui::SetNextItemWidth(-FLT_MIN);
    if (ImGui::InputScalar("##CondScalar", ImGuiDataType_Double, &conductivity, &cond_step, nullptr, tr("%.4f W/(m·K)").data()))
    {
        if (conductivity < min_cond) conductivity = min_cond;
        if (conductivity > max_cond) conductivity = max_cond;
        _editing_material.thermal_conductivity = conductivity;
    }

    ImGui::TextUnformatted(tr("Volumetric Density:").data());
    ImGui::SameLine(220.0f);
    ImGui::SetNextItemWidth(-FLT_MIN);
    if (ImGui::InputScalar("##DensityScalar", ImGuiDataType_Double, &density, &dens_step, nullptr, tr("%.1f kg/m³").data()))
    {
        if (density < min_dens) density = min_dens;
        if (density > max_dens) density = max_dens;
        _editing_material.density = density;
    }

    ImGui::Spacing();
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

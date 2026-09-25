#include "wall_context_panel.h"

#include <imgui.h>

#include "translate.h"

using namespace gui::localization;

gui::wall_context_panel::wall_context_panel(ui_dispatcher::wall_context_panel_dispatcher& wcp_dispatcher)
    : _wcp_dispatcher { wcp_dispatcher }
{
}

void gui::wall_context_panel::process_frame()
{
    auto shown_control = _wcp_dispatcher.show.take();
    if (shown_control)
    {
        _shown = shown_control.value();
    }

    if (!_shown)
    {
        return;
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize |
                                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav;

    ImVec2 appearance_pos = ImVec2(0.0f, 20.0f);
    ImGui::SetNextWindowPos(appearance_pos, ImGuiCond_Always);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.0f);

    if (ImGui::Begin("##WallContextHUD", nullptr, flags))
    {
        _is_mouse_hovering = ImGui::IsWindowHovered(
            ImGuiHoveredFlags_RootWindow | 
            ImGuiHoveredFlags_ChildWindows | 
            ImGuiHoveredFlags_AllowWhenBlockedByPopup
        );

        ImGui::TextDisabled("%s", tr("Wall Layout Options").data());
        ImGui::Separator();

        const char* preview_type_name = "Ext_Brick_300"; 
        if (ImGui::BeginCombo("##WallTypeSelect", preview_type_name, ImGuiComboFlags_HeightRegular))
        {
            if (ImGui::Selectable("Ext_Brick_300", true)) { /* select entry */ }
            if (ImGui::Selectable("Int_Partition_100", false)) { /* select entry */ }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        
        if (ImGui::Button("..."))
        {
            _should_open_wall_layers_editor_modal = true;
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("%s", tr("Open Compound Layer Editor").data());
        }
    }
    ImGui::End();
    
    ImGui::PopStyleVar();
}

bool gui::wall_context_panel::take_open_wall_layers_editor_trigger()
{
    auto res = _should_open_wall_layers_editor_modal;
    _should_open_wall_layers_editor_modal = false;

    return res;
}

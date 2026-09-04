#include "cmd_panel.h"

using namespace gui;

namespace
{
    int FilterTildeCallback(ImGuiInputTextCallbackData* data)
    {
        if (data->EventChar == '~' || data->EventChar == '`')
        {
            return 1; 
        }
        return 0;
    }
}

gui::cmd_panel::cmd_panel()
{
    _command_buffer[0] = '\0';
}

void cmd_panel::process_frame()
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    constexpr float slider_size = 5.0f;

    const float min_height = 50.0f;
    const float max_height = vp->WorkSize.y - slider_size - 20.0f;
    if (_height < min_height) _height = min_height;
    if (_height > max_height) _height = max_height;

    float panel_y = vp->WorkPos.y + vp->WorkSize.y - _height;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoNav;


    ImGui::SetNextWindowPos(ImVec2 { vp->WorkPos.x, panel_y });
    ImGui::SetNextWindowSize(ImVec2 { vp->WorkSize.x, _height });

    ImGui::Begin("Command Panel", nullptr, flags);

    ImVec2 content_cursor_backup = ImGui::GetCursorPos();

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
     
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImGui::InvisibleButton("###cmd_panel_splitter_handle", ImVec2(-1.0f, slider_size));
    
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    {
        _height -= ImGui::GetIO().MouseDelta.y;
    }

    if (ImGui::IsItemHovered() || ImGui::IsItemActive())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
    }
    
    ImGui::PopStyleVar(2);


    ImGui::SetCursorPos(ImVec2(content_cursor_backup.x, content_cursor_backup.y + slider_size));


    float reserved_bottom_space = 35.0f;
    float main_content_height = _height - slider_size - reserved_bottom_space;

    if (main_content_height > 0.0f) 
    {
        ImGui::PushItemWidth(-1.0f);
        
        ImGui::InputTextMultiline(
            "##cmd_history", 
            const_cast<char*>(_log.c_str()), 
            _log.size() + 1, 
            ImVec2(-1.0f, main_content_height), 
            ImGuiInputTextFlags_ReadOnly
        );
                                  
        ImGui::PopItemWidth();
    }


    ImGui::SetCursorPosY(_height - reserved_bottom_space + 5.0f);
    
    ImGui::PushItemWidth(-1.0f); 

    if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent, false))
    {
        ImGui::SetKeyboardFocusHere(0); 
    }
    
    ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCharFilter;
    if (ImGui::InputText("##cmd_input", _command_buffer.data(), _command_buffer.size(), input_flags, FilterTildeCallback))
    {
        if (_command_buffer[0])
        {
            _log += "> ";
            _log += _command_buffer.data();
            _log += "\n";

        }
        _command_buffer[0] = '\0'; 
        ImGui::SetKeyboardFocusHere(-1); 
    }
    ImGui::PopItemWidth();
    
    
    _is_mouse_hovering = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

    ImGui::End();
}

bool gui::cmd_panel::is_mouse_hovering() const
{
    return _is_mouse_hovering;
}

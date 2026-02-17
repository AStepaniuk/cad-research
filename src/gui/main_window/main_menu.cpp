#include "main_menu.h"

#include <iostream>
#include <imgui.h>

using namespace gui;

void main_menu::process_frame()
{
    _choosen_item = item::none;

    if (ImGui::BeginMainMenuBar())
    {
        _is_mouse_hovering = ImGui::IsAnyItemHovered() || ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                // Handle open
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                // Handle save
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))
            {
                // glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Draw"))
        {
            if (ImGui::MenuItem("Wall", "W"))
            {
                _choosen_item = item::add_wall;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

bool main_menu::is_mouse_hovering() const
{
    return _is_mouse_hovering;
}

gui::main_menu::item gui::main_menu::choosen_item() const
{
    return _choosen_item;
}

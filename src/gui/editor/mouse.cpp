#include "mouse.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace gui::editor;

gui::editor::mouse::mouse(GLFWwindow *window)
    : _window { window }
{
}

void mouse::process_frame(bool control_mouse)
{
    ImVec2 mp = ImGui::GetIO().MousePos;

    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);

    if (control_mouse && mp.x > 0 && mp.x < width && mp.y > 0 && mp.y < height)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        ImU32 mouse_color = IM_COL32(192, 192, 192, 255);
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        draw_list->AddLine(ImVec2 { mp.x, 0 }, ImVec2 { mp.x, static_cast<float>(height) }, mouse_color, 1.0f);
        draw_list->AddLine(ImVec2 { 0, mp.y }, ImVec2 { static_cast<float>(width), mp.y }, mouse_color, 1.0f);
    }
    else
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

}

#pragma once

#include <backends/imgui_impl_glfw.h>

#include "main_menu.h"
#include "document.h"
#include "cmd_ast.h"
#include "floor_editor.h"
#include "ui.h"

namespace gui
{
    class workspace
    {
        ui_controller::ui& _ui;

        main_menu& _main_menu;

        doc::document _document;
        editor::floor_editor _editor;

    public:
        workspace(GLFWwindow* window, main_menu& mm, ui_controller::ui& ui);

        bool execute_instruction(const cmd_parser::instruction& instruction);
        void process_frame(bool mouse_in_workspace);
    };
}
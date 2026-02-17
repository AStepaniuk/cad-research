#pragma once

#include <backends/imgui_impl_glfw.h>

#include "main_menu.h"
#include "document.h"
#include "floor_editor.h"

namespace gui {
    class workspace
    {
        main_menu& _main_menu;

        doc::document _document;
        editor::floor_editor _editor;

    public:
        workspace(GLFWwindow* window, main_menu& mm);

        void process_frame(bool mouse_in_workspace);
    };
}
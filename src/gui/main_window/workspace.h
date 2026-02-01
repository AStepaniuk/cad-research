#pragma once

#include <backends/imgui_impl_glfw.h>

#include "document.h"
#include "floor_editor.h"

namespace gui {
    class workspace
    {
        doc::document _document;
        editor::floor_editor _editor;

    public:
        workspace(GLFWwindow* window);

        void process_frame(bool mouse_in_workspace);
    };
}
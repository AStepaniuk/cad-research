#pragma once

#include <backends/imgui_impl_glfw.h>

#include "document.h"

#include "floor_view.h"
#include "mouse.h"

namespace gui {
    class workspace
    {
        doc::document _document;

        floor_view _view;
        editor::mouse _mouse;


    public:
        workspace(GLFWwindow* window);

        void process_frame(bool mouse_in_workspace);
    };
}
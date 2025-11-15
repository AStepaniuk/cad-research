#pragma once

#include <backends/imgui_impl_glfw.h>

namespace gui { namespace editor {
    class mouse
    {
        GLFWwindow* _window;

    public:
        mouse(GLFWwindow* window);

        void process_frame(bool control_mouse);
    };
}}

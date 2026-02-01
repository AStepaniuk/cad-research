#pragma once

#include "document.h"
#include "floor_view.h"
#include "mouse.h"

namespace gui { namespace editor {
    class floor_editor
    {
        doc::document& _document;

        floor_view _view;
        editor::mouse _mouse;


    public:
        floor_editor(GLFWwindow* window, doc::document& doc);

        void process_frame(bool mouse_in_workspace);
    };
}}

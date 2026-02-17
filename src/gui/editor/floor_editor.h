#pragma once

#include "document.h"
#include "floor_view.h"
#include "mouse.h"
#include "calc_tools.h"

#include "operation/operation_idle.h"
#include "operation/operation_add_wall.h"

namespace gui { namespace editor {
    class floor_editor
    {
        doc::document& _document;

        floor_view _view;
        editor::mouse _mouse;
        calc_tools _tools;

        operation::operation_idle _operation_idle;
        operation::operation_add_wall _operation_add_wall;

        operation::i_operation* _current_operation;

        void switch_operation(operation::i_operation *op);

    public:
        floor_editor(GLFWwindow* window, doc::document& doc);

        void process_frame(bool mouse_in_workspace);

        void start_operation_add_wall();
    };
}}

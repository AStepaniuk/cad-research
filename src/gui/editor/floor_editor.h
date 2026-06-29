#pragma once

#include "document.h"
#include "floor_view.h"
#include "mouse.h"
#include "calc_tools.h"

#include "operation/operation_idle.h"
#include "operation/operation_add_wall.h"

namespace gui::editor {
    class floor_editor
    {
        doc::document& _document;

        calc_tools _tools;

        floor_view _view;
        editor::mouse _mouse;
        
        operation::operation_idle _operation_idle;
        operation::operation_add_wall _operation_add_wall;

        operation::i_operation* _current_operation { nullptr };

        void switch_operation(operation::i_operation *op);
        void reset_selection();

    public:
        floor_editor(GLFWwindow* window, doc::document& doc);

        void process_frame(bool mouse_in_workspace);

        void start_operation_add_wall();
        void undo();
        void redo();
    };
}

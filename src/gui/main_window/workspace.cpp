#include "workspace.h"

#include <imgui.h>

using namespace gui;

workspace::workspace(GLFWwindow* window, main_menu& mm)
    : _main_menu { mm }
    , _document {}
    , _editor { window, _document }
{
}

void workspace::process_frame(bool mouse_in_workspace)
{
    if (_main_menu.choosen_item() == main_menu::item::add_wall)
    {
        _editor.start_operation_add_wall();
    }
    else if (_main_menu.choosen_item() == main_menu::item::undo)
    {
        _editor.undo();
    }
    else if (_main_menu.choosen_item() == main_menu::item::redo)
    {
        _editor.redo();
    }

    _editor.process_frame(mouse_in_workspace);
}

#include "workspace.h"

using namespace gui;

workspace::workspace(GLFWwindow* window, main_menu& mm, ui_dispatcher::root_ui_dispatcher& ui_dispatcher)
    : _ui_dispatcher { ui_dispatcher }
    , _main_menu { mm }
    , _document {}
    , _editor { window, _document, _ui_dispatcher }
    , _wme {{ _document.model.data().items<domain::plan::model::shape::wall_material_definition>() }}
{
}

bool gui::workspace::execute_instruction(const cmd_parser::instruction& instruction)
{
    return _editor.execute_instruction(instruction);
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
    else if (_main_menu.choosen_item() == main_menu::item::edit_wall_materials)
    {
        _wme.open({});
    }

    _wme.process_frame();
    _editor.process_frame(mouse_in_workspace && !_wme.is_active());
}

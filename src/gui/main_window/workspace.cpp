#include "workspace.h"

#include <imgui.h>

#include "default_floor_generator.h"
#include "wall_calculator.h"
#include "constraints_calculator.h"

using namespace gui;

workspace::workspace(GLFWwindow* window, main_menu& mm)
    : _main_menu { mm }
    , _document {}
    , _editor { window, _document }
{
    domain::plan::generator::default_floor_generator fg;
    fg.generate_floor(_document.model);

    domain::plan::calculator::wall_calculator wc { _document.model };
    wc.recalculate_all_walls();

    corecad::calculator::constraints_calculator cc;
    cc.recalculate_all(_document.model.parameters(), _document.model.points());

    wc.recalculate_all_walls();
}

void workspace::process_frame(bool mouse_in_workspace)
{
    if (_main_menu.choosen_item() == main_menu::item::add_wall)
    {
        _editor.start_operation_add_wall();
    }

    _editor.process_frame(mouse_in_workspace);
}

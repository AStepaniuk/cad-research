#include "workspace.h"

#include <imgui.h>

#include "default_floor_generator.h"
#include "wall_calculator.h"
#include "constraints_calculator.h"

using namespace gui;

workspace::workspace(GLFWwindow* window)
    : _document {}
    , _editor { window, _document }
{
    domain::plan::generator::default_floor_generator fg;
    fg.generate_floor(_document.model);

    domain::plan::calculator::wall_calculator wc;
    wc.recalculate_all_walls(_document.model);

    corecad::calculator::constraints_calculator cc;
    cc.recalculate_all(_document.model.parameters(), _document.model.points());

    wc.recalculate_all_walls(_document.model);
}

void workspace::process_frame(bool mouse_in_workspace)
{
    _editor.process_frame(mouse_in_workspace);
}

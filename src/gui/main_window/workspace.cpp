#include "workspace.h"

#include <imgui.h>

#include "default_floor_generator.h"
#include "wall_calculator.h"

using namespace gui;

workspace::workspace(GLFWwindow* window)
    : _mouse { window }
{
    domain::plan::generator::default_floor_generator fg;
    fg.generate_floor(_document.model);

    domain::plan::calculator::wall_calculator wc;
    wc.recalculate_all_walls(_document.model);
}

void workspace::process_frame(bool mouse_in_workspace)
{
    const auto mp = ImGui::GetIO().MousePos;

    const auto mw = ImGui::GetIO().MouseWheel;
    if (mw != 0)
    {
        auto scale_factor = 1.0f;
        constexpr auto scale_step = 1.1f;
        
        for (int i = mw; i > 0; i--) scale_factor *= scale_step;
        for (int i = mw; i < 0; i++) scale_factor /= scale_step;

        _view.x_scale(_view.x_scale() * scale_factor);
        _view.y_scale(_view.y_scale() * scale_factor);

        _view.x_offset(_view.x_offset() * scale_factor - mp.x * (scale_factor - 1.0f));
        _view.y_offset(_view.y_offset() * scale_factor - mp.y * (scale_factor - 1.0f));
    }

    if (ImGui::GetIO().MouseDown[2])
    {
        const auto delta = ImGui::GetIO().MouseDelta;

        _view.x_offset(_view.x_offset() + delta.x);
        _view.y_offset(_view.y_offset() + delta.y);
    }

    _view.render(_document);

    _mouse.process_frame(mouse_in_workspace);
}

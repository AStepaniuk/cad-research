#include "floor_editor.h"

using namespace gui::editor;

floor_editor::floor_editor(GLFWwindow *window, doc::document& doc)
    : _document { doc }
    , _view { doc }
    , _mouse { window }
    , _operation_idle { _document, _view }
{
    _current_operation = &_operation_idle;
}

void floor_editor::process_frame(bool mouse_in_workspace)
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

    const auto mouse_delta = ImGui::GetIO().MouseDelta;
    if (ImGui::GetIO().MouseDown[2])
    {
        _view.x_offset(_view.x_offset() + mouse_delta.x);
        _view.y_offset(_view.y_offset() + mouse_delta.y);
    }

    if (_current_operation)
    {
        if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f)
        {
            _current_operation->handle_mouse_move(mp.x, mp.y);
        }
    }

    _view.render();

    _mouse.process_frame(mouse_in_workspace);
}

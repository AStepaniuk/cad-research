#include "floor_editor.h"

#include "default_floor_generator.h"

using namespace gui::editor;
using namespace domain::plan::model;
using namespace domain::plan::model::shape;

floor_editor::floor_editor(GLFWwindow *window, doc::document &doc)
    :_document{ doc }
    , _tools { _document.model }
    , _view { doc, _tools.point_resolver }
    , _mouse { window }
    , _operation_idle { _document, _view, _tools }
    , _operation_add_wall { _document, _view, _tools }
{
    domain::plan::generator::default_floor_generator fg;
    fg.generate_floor(_document.model);

    _tools.wall_calculator.recalculate_all_walls();

    _tools.constraint_calculator.recalculate_all(_document.model.data().items<floor::constraint_t>());

    _tools.wall_calculator.recalculate_all_walls();

    _document.model.history().commit("Initial setup");

    switch_operation(&_operation_idle);
}

void floor_editor::switch_operation(operation::i_operation *op)
{
    if (_current_operation)
    {
        _current_operation->stop();
        _document.model.history().cancel();

        _tools.wall_calculator.recalculate_all_walls();
    }

    _current_operation = op;
    _current_operation->start();
}

void gui::editor::floor_editor::reset_selection()
{
    _document.selected_walls.clear();
    _document.active_walls.clear();
    _document.hovered_handle = std::nullopt;
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
            _current_operation->mouse_move(mp.x, mp.y);
        }

        if (ImGui::GetIO().MouseClicked[0])
        {
            _current_operation->left_mouse_click(mp.x, mp.y);
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            switch_operation(&_operation_idle);
        }
    }

    _view.render();

    _mouse.process_frame(mouse_in_workspace);
}

void floor_editor::start_operation_add_wall()
{
    switch_operation(&_operation_add_wall);
}

void gui::editor::floor_editor::undo()
{
    switch_operation(&_operation_idle);

    _document.model.history().undo();
    _tools.wall_calculator.recalculate_all_walls();
}

void gui::editor::floor_editor::redo()
{
    switch_operation(&_operation_idle);
    
    _document.model.history().redo();
    _tools.wall_calculator.recalculate_all_walls();
}

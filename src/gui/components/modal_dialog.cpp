#include "modal_dialog.h"

using namespace gui::components;

modal_dialog::modal_dialog(ImVec2 default_size, ImVec2 min_size, ImVec2 max_size)
    : _default_size { default_size }
    , _min_size { min_size }
    , _max_size { max_size }
{}

void modal_dialog::open()
{
    _should_open_popup = true;
    _is_active = true;
}

void modal_dialog::close()
{
    _is_active = false;
}

modal_dialog_scope_handle modal_dialog::begin(const char *title)
{
    if (!_is_active && !_is_open) return modal_dialog_scope_handle {};

    if (_should_open_popup)
    {
        ImGui::OpenPopup(title);
        _should_open_popup = false;
    }

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImVec2 dynamic_max = _max_size;
    
    if (main_viewport)
    {
        constexpr float screen_margin = 40.0f;
        if (dynamic_max.x > main_viewport->Size.x - screen_margin) dynamic_max.x = main_viewport->Size.x - screen_margin;
        if (dynamic_max.y > main_viewport->Size.y - screen_margin) dynamic_max.y = main_viewport->Size.y - screen_margin;
    }

    ImGui::SetNextWindowSizeConstraints(_min_size, dynamic_max);
    ImGui::SetNextWindowSize(_default_size, ImGuiCond_FirstUseEver);

    _is_open = ImGui::BeginPopupModal(title, &_is_active, ImGuiWindowFlags_AlwaysAutoResize);

    return _is_open ? modal_dialog_scope_handle { *this } : modal_dialog_scope_handle {};
}

void modal_dialog::end()
{
    if (_is_open)
    {
        if (!_is_active || ImGui::IsKeyPressed(ImGuiKey_Escape))
        {
            ImGui::CloseCurrentPopup();
            _is_active = false;
        }
        
        ImGui::EndPopup();

        _is_open = false;
    }
}

bool modal_dialog::is_active() const
{
    return _is_active;
}

bool gui::components::modal_dialog::is_open() const
{
    return _is_open;
}

modal_dialog_scope_handle::modal_dialog_scope_handle(modal_dialog &dialog)
    : _dialog { &dialog }
{
}

modal_dialog_scope_handle::~modal_dialog_scope_handle()
{
    if (_dialog)
    {
        _dialog->end();
    }
}

modal_dialog_scope_handle::modal_dialog_scope_handle(modal_dialog_scope_handle &&other) noexcept
    : _dialog { other._dialog }
{
    other._dialog = nullptr;
}

modal_dialog_scope_handle &modal_dialog_scope_handle::operator=(modal_dialog_scope_handle &&other) noexcept
{
    if (this != &other)
    {
        if (_dialog) _dialog->end();
        _dialog = other._dialog;
        other._dialog = nullptr;
    }
    return *this;
}

modal_dialog_scope_handle::operator bool() const
{
    return _dialog && _dialog->is_open();
}

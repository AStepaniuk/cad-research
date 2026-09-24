#pragma once

#include "ui_wall_context_panel.h"

namespace gui
{
    class wall_context_panel
    {
    public:
        wall_context_panel(ui_controller::ui_wall_context_panel& ui);

        void process_frame();

        bool is_mouse_hovering() const { return _is_mouse_hovering; }
        bool take_open_wall_layers_editor_trigger();

    private:
        ui_controller::ui_wall_context_panel& _ui;

        bool _shown = false;
        bool _is_mouse_hovering = false;
        bool _should_open_wall_layers_editor_modal = false;
    };
}
        
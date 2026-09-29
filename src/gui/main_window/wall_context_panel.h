#pragma once

#include "wall_context_panel_dispatcher.h"

namespace gui
{
    class wall_context_panel
    {
    public:
        wall_context_panel(ui_dispatcher::wall_context_panel_dispatcher& wcp_dispatcher);

        void process_frame();

        bool is_mouse_hovering() const { return _is_mouse_hovering; }

    private:
        ui_dispatcher::wall_context_panel_dispatcher& _wcp_dispatcher;

        bool _shown = false;
        bool _is_mouse_hovering = false;
        bool _should_open_wall_layers_editor_modal = false;
    };
}
        
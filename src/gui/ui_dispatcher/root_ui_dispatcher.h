#pragma once

#include "wall_context_panel_dispatcher.h"
#include "wall_layers_editor_dispatcher.h"
#include "wall_materials_editor_dispatcher.h"

namespace gui::ui_dispatcher
{
    struct root_ui_dispatcher
    {
        wall_context_panel_dispatcher wall_context_panel;
        wall_layers_editor_dispatcher wall_layers_editor;
        wall_materials_editor_dispatcher wall_materials_editor;
    };
}

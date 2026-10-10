#pragma once

#include <vector>
#include <imgui.h>

#include "registry.h"
#include "wall_layer.h"
#include "wall_material_definition.h"

namespace gui
{
    class wall_profile_preview_canvas
    {
    public:
        using layer_t = domain::plan::model::shape::wall_layer;
        using material_registry_t = corecad::model::registry<domain::plan::model::shape::wall_material_definition>;

        struct context_data
        {
            const std::vector<layer_t>& editing_layers;
            const material_registry_t& materials_lookup;
        };

        wall_profile_preview_canvas(context_data ctx);

        void render(const char* str_id, float target_width, float target_height);

        void reset_zoom_view(ImVec2 canvas_size);

    private:
        context_data _ctx;

        float _zoom_factor = 2.0f;
        ImVec2 _pan_offset = {0.0f, 0.0f};
    };
}

#pragma once

#include <optional>
#include <vector>

#include <imgui.h>

#include "document.h"
#include "coord_translator.h"
#include "walls_view.h"
#include "constraints_view.h"

namespace gui {
    class floor_view
    {
        const doc::document& _document;

        translator_t _translator;

        walls_view _walls_view;
        constraints_view _constraints_view;

    public:
        floor_view(const doc::document& doc);

        int x_offset() const;
        void x_offset(int val);

        int y_offset() const;
        void y_offset(int val);

        float x_scale() const;
        void x_scale(float val);

        float y_scale() const;
        void y_scale(float val);

        void render();

        abstract_point to_model(float screen_x, float screen_y) const;
        abstract_point model_interaction_tolerance() const;

        std::optional<domain::plan::model::wall::index_t> get_wall(float screen_x, float screen_y) const;
        std::vector<domain::plan::model::wall_axis_point::index_t> get_handles(float screen_x, float screen_y) const;
    };
}

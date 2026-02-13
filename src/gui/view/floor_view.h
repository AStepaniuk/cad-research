#pragma once

#include <optional>
#include <vector>

#include <imgui.h>

#include "document.h"
#include "coord_translator.h"

namespace gui {
    class floor_view
    {
        const doc::document& _document;

        coord_translator _translator;

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

        corecad::model::vector2d to_model(float screen_x, float screen_y) const;

        std::optional<domain::plan::model::wall::index_t> get_wall(float screen_x, float screen_y) const;
        std::vector<corecad::model::vector2d::index_t> get_handles(float screen_x, float screen_y) const;

    private:
        std::vector<ImVec2> to_view_polygon(const domain::plan::model::wall& w) const;

        bool is_point_in_polygon(const ImVec2& point, const std::vector<ImVec2>& polygon) const;
    };
}
#pragma once

#include <optional>

#include <imgui.h>

#include "document.h"

namespace gui {
    class floor_view
    {
        int _x_offset = 0;
        int _y_offset = 0;

        float _x_scale = 0.1;
        float _y_scale = 0.1;

        const doc::document& _document;

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
        std::optional<domain::plan::model::wall::index_t> get_wall(float screen_x, float screen_y) const;


    private:
        ImVec2 to_view(const corecad::model::vector2d& p) const;
        std::vector<ImVec2> to_view_polygon(const domain::plan::model::wall& w) const;

        bool is_point_in_polygon(const ImVec2& point, const std::vector<ImVec2>& polygon) const;
    };
}
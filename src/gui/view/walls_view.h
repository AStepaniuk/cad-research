#pragma once

#include <optional>
#include <vector>

#include <imgui.h>

#include "document.h"
#include "coord_translator.h"

namespace gui {
    class walls_view
    {
        const doc::document& _document;

        const translator_t& _translator;

    public:
        walls_view(const doc::document& doc, const translator_t& t);

        void render(ImDrawList* draw_list);

        std::optional<domain::plan::model::shape::wall::index_t> get_wall(float screen_x, float screen_y) const;
        std::vector<domain::plan::model::shape::wall_axis_point::index_t> get_handles(float screen_x, float screen_y) const;

    private:
        void render_line(
            ImDrawList* draw_list,
            domain::plan::model::shape::wall_border_line::index_t lid,
            float line_thickness,
            ImU32 line_color
        ) const;
        std::vector<ImVec2> to_view_polygon(const domain::plan::model::shape::wall& w) const;

        bool is_point_in_polygon(const ImVec2& point, const std::vector<ImVec2>& polygon) const;
    };
}
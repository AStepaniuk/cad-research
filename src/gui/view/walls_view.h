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

        const coord_translator& _translator;

    public:
        walls_view(const doc::document& doc, const coord_translator& t);

        void render(ImDrawList* draw_list);

        std::optional<domain::plan::model::wall::index_t> get_wall(float screen_x, float screen_y) const;
        std::vector<corecad::model::vector2d::index_t> get_handles(float screen_x, float screen_y) const;

    private:
        std::vector<ImVec2> to_view_polygon(const domain::plan::model::wall& w) const;

        bool is_point_in_polygon(const ImVec2& point, const std::vector<ImVec2>& polygon) const;
    };
}
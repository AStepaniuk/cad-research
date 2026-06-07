#pragma once

#include "document.h"
#include "coord_translator.h"

namespace gui {
    class constraints_view
    {
    public:
        using coord_translator_t = coord_translator<
            domain::plan::model::shape::wall_axis_point,
            domain::plan::model::shape::wall_border_point
        >;

    private:
        const doc::document& _document;

        const coord_translator_t& _translator;

    public:
        constraints_view(const doc::document& doc, const coord_translator_t & t);

        void render(ImDrawList* draw_list);

    private:
        void draw_constraint(ImDrawList* draw_list, const domain::plan::model::floor::constraint_t& c, ImU32 color);
    };
}
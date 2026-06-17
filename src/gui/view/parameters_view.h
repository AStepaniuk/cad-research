#pragma once

#include "document.h"
#include "coord_translator.h"
#include "point_resolver.h"

namespace gui {
    class parameters_view
    {
    public:
        using coord_translator_t = coord_translator<
            domain::plan::model::shape::wall_axis_point,
            domain::plan::model::shape::wall_border_point
        >;

    private:
        const doc::document& _document;

        const coord_translator_t& _translator;
        domain::plan::resolver::point_resolver& _point_resolver;

    public:
        parameters_view(
            const doc::document& doc,
            const coord_translator_t & t,
            domain::plan::resolver::point_resolver& pr
        );

        void render(ImDrawList* draw_list);

    private:
        void draw_parameter(ImDrawList* draw_list, const domain::plan::model::parameter::parameter& p, ImU32 color);
    };
}
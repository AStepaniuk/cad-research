#pragma once

#include "document.h"
#include "coord_translator.h"
#include "point_resolver.h"

namespace gui {
    class parameters_view
    {
        const doc::document& _document;

        const translator_t& _translator;
        domain::plan::resolver::point_resolver& _point_resolver;

    public:
        parameters_view(
            const doc::document& doc,
            const translator_t & t,
            domain::plan::resolver::point_resolver& pr
        );

        void render(ImDrawList* draw_list);

    private:
        void draw_parameter(ImDrawList* draw_list, const domain::plan::model::parameter::parameter& p, ImU32 color);
    };
}
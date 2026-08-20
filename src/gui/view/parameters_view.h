#pragma once

#include "document.h"
#include "coord_translator.h"

namespace gui
{
    class parameters_view
    {
        const doc::document& _document;

        const translator_t& _translator;

    public:
        parameters_view(
            const doc::document& doc,
            const translator_t & t
        );

        void render(ImDrawList* draw_list);

    private:
        void draw_parameter(ImDrawList* draw_list, const domain::plan::model::parameter::parameter& p, ImU32 color);

        ImVec2 to_view(const domain::plan::model::shape::wall_point_id_t& pid) const;
    };
}
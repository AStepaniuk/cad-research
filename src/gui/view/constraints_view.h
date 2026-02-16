#pragma once

#include "document.h"
#include "coord_translator.h"

namespace gui {
    class constraints_view
    {
        const doc::document& _document;

        const coord_translator& _translator;

    public:
        constraints_view(const doc::document& doc, const coord_translator& t);

        void render(ImDrawList* draw_list);
    };
}
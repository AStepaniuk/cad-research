#pragma once

#include "document.h"
#include "calc_tools.h"

#include <vector>
#include <utility>

namespace gui::editor::snap {
    class snap_processor
    {
    public:
        snap_processor(doc::document& doc, calc_tools& ct);

        bool process();

    private:
        doc::document& _document;
        calc_tools& _calc_tools;
    };
}


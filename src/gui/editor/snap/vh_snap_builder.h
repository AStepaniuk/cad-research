#pragma once

#include "i_snap_builder.h"

#include "document.h"
#include "floor_view.h"

#include <vector>
#include <utility>

namespace gui { namespace editor { namespace snap {
    class vh_snap_builder : public i_snap_builder
    {
    public:
        vh_snap_builder(doc::document& doc, floor_view& v);

        void calculate_snaps(float view_pos_x, float view_pos_y) override;

    private:
        doc::document& _document;
        floor_view& _floor_view;
    };
}}}

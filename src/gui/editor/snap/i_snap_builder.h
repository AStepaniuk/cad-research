#pragma once

#include <vector>
#include <utility>

#include "document.h"

namespace gui { namespace editor { namespace snap {
    class i_snap_builder
    {
    public:
        virtual void calculate_snaps(float view_pos_x, float view_pos_y) = 0;
    };
}}}

#pragma once

#include <imgui.h>
#include "document.h"

namespace gui {
    class floor_view
    {
        int _x_offset = 0;
        int _y_offset = 0;

        float _x_scale = 0.1;
        float _y_scale = 0.1;

    public:
        int x_offset() const;
        void x_offset(int val);

        int y_offset() const;
        void y_offset(int val);

        float x_scale() const;
        void x_scale(float val);

        float y_scale() const;
        void y_scale(float val);

        void render(const doc::document& doc);

    private:
        ImVec2 to_view(const corecad::model::vector2d& p) const;
    };
}
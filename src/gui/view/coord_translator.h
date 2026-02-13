#pragma once

#include <imgui.h>

#include "registry.h"
#include "vector2d.h"

namespace gui {
    class coord_translator
    {
    public:
        coord_translator(const corecad::model::registry<corecad::model::vector2d>& points);

        int x_offset() const;
        void x_offset(int val);

        int y_offset() const;
        void y_offset(int val);

        float x_scale() const;
        void x_scale(float val);

        float y_scale() const;
        void y_scale(float val);

        corecad::model::vector2d to_model(const ImVec2 screen) const;
        ImVec2 to_view(const corecad::model::vector2d& p) const;
        ImVec2 to_view(corecad::model::vector2d::index_t index) const;

    private:
        const corecad::model::registry<corecad::model::vector2d>& _points;

        int _x_offset = 0;
        int _y_offset = 0;

        float _x_scale = 0.1;
        float _y_scale = 0.1;
    };
}

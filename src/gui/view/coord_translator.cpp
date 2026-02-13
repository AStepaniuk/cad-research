#include "coord_translator.h"

using namespace gui;
using namespace corecad::model;

namespace
{
    constexpr float MinScale = 0.0001;
}

coord_translator::coord_translator(const registry<vector2d> &points)
    : _points { points }
{
}

int coord_translator::x_offset() const
{
    return _x_offset;
}

void coord_translator::x_offset(int val)
{
    _x_offset = val;
}

int coord_translator::y_offset() const
{
    return _y_offset;
}

void coord_translator::y_offset(int val)
{
    _y_offset = val;
}

float coord_translator::x_scale() const
{
    return _x_scale;
}

void coord_translator::x_scale(float val)
{
    if (val >= MinScale)
    {
        _x_scale = val;
    }
    else
    {
        _x_scale = MinScale;
    }
}

float coord_translator::y_scale() const
{
    return _y_scale;
}

void coord_translator::y_scale(float val)
{
    if (val >= MinScale)
    {
        _y_scale = val;
    }
    else
    {
        _y_scale = MinScale;
    }
}

vector2d coord_translator::to_model(const ImVec2 screen) const
{
    return vector2d
    {
        (static_cast<double>(screen.x) - _x_offset) / _x_scale,
        (static_cast<double>(screen.y) - _y_offset) / _y_scale
    };
}

ImVec2 coord_translator::to_view(const corecad::model::vector2d &p) const
{
    return ImVec2
    {
        static_cast<float>((p.x * _x_scale) + _x_offset),
        static_cast<float>((p.y * _y_scale) + _y_offset)
    };
}

ImVec2 coord_translator::to_view(corecad::model::vector2d::index_t index) const
{
    const auto& p = _points.get(index);

    return to_view(p);
}

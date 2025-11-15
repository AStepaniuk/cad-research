#include "floor_view.h"

using namespace gui;

int floor_view::x_offset() const
{
    return _x_offset;
}

void gui::floor_view::x_offset(int val)
{
    _x_offset = val;
}

int gui::floor_view::y_offset() const
{
    return _y_offset;
}

void gui::floor_view::y_offset(int val)
{
    _y_offset = val;
}

float gui::floor_view::x_scale() const
{
    return _x_scale;
}

void gui::floor_view::x_scale(float val)
{
    _x_scale = val;
}

float gui::floor_view::y_scale() const
{
    return _y_scale;
}

void gui::floor_view::y_scale(float val)
{
    _y_scale = val;
}

void gui::floor_view::render(const doc::document& doc)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    for (const auto& wp : doc.model.walls())
    {
        const auto& w = wp.second;

        const auto& ps = doc.model.points().get(w.start);
        const auto& pe = doc.model.points().get(w.end);

        draw_list->AddLine(to_view(ps), to_view(pe), IM_COL32(96, 96, 96, 255), 1.0f);

        const auto& sl = doc.model.points().get(w.start_left);
        const auto& sr = doc.model.points().get(w.start_right);
        const auto& el = doc.model.points().get(w.end_left);
        const auto& er = doc.model.points().get(w.end_right);

        draw_list->AddLine(to_view(sl), to_view(el), IM_COL32(192, 192, 255, 255), 2.0f);
        draw_list->AddLine(to_view(sr), to_view(er), IM_COL32(192, 192, 255, 255), 2.0f);

        if (w.start_joints == 0)
        {
            draw_list->AddLine(to_view(sl), to_view(sr), IM_COL32(192, 192, 255, 255), 2.0f);
        }
        if (w.end_joints == 0)
        {
            draw_list->AddLine(to_view(el), to_view(er), IM_COL32(192, 192, 255, 255), 2.0f);
        }
    }
}

ImVec2 gui::floor_view::to_view(const corecad::model::vector2d &p) const
{
    return ImVec2
    {
        static_cast<float>((p.x * _x_scale) + _x_offset),
        static_cast<float>((p.y * _y_scale) + _y_offset)
    };
}

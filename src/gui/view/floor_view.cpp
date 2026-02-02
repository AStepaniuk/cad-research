#include "floor_view.h"

using namespace gui;
using namespace domain::plan::model;

namespace
{
    namespace Styles
    {
        constexpr auto WallLineColor = IM_COL32(192, 192, 192, 255);
        constexpr auto WallLineThickness = 2.0f;

        constexpr auto WallMidLineColor = IM_COL32(96, 96, 96, 255);
        constexpr auto WallMidLineThickness = 1.0f;


        constexpr auto WallHoveredLineColor = IM_COL32(224, 224, 224, 255);
        constexpr auto WallHoveredFillColor = IM_COL32(40, 40, 40, 255);
        constexpr auto WallHoveredLineThickness = 2.5f;
                
        constexpr auto WallHoveredMidLineColor = IM_COL32(96, 96, 96, 255);
        constexpr auto WallHoveredMidLineThickness = 1.0f;
    }
}

gui::floor_view::floor_view(const doc::document &doc)
    : _document { doc }
{
}

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

void gui::floor_view::render()
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    for (const auto& wp : _document.model.walls())
    {
        const auto& w = wp.second;
        const auto polygon = to_view_polygon(w);

        if (_document.hovered_wall_id == w.index)
        {
            draw_list->AddConvexPolyFilled(polygon.data(), polygon.size(), Styles::WallHoveredFillColor);
            draw_list->AddPolyline(polygon.data(), polygon.size(), Styles::WallHoveredLineColor, ImDrawFlags_Closed, Styles::WallHoveredLineThickness);
            draw_list->AddLine(polygon[0], polygon[3], Styles::WallHoveredMidLineColor, Styles::WallHoveredMidLineThickness);
        }
        else
        {
            draw_list->AddLine(polygon[1], polygon[2], Styles::WallLineColor, Styles::WallLineThickness);
            draw_list->AddLine(polygon[4], polygon[5], Styles::WallLineColor, Styles::WallLineThickness);
            if (w.start_joints == 0)
            {
                draw_list->AddLine(polygon[1], polygon[5], Styles::WallLineColor, Styles::WallLineThickness);
            }
            if (w.end_joints == 0)
            {
                draw_list->AddLine(polygon[2], polygon[4], Styles::WallLineColor, Styles::WallLineThickness);
            }

            draw_list->AddLine(polygon[0], polygon[3], Styles::WallMidLineColor, Styles::WallMidLineThickness);
        }
    }
}

std::optional<wall::index_t> gui::floor_view::get_wall(float screen_x, float screen_y) const
{
    ImVec2 p { screen_x, screen_y };

    for (const auto& wp : _document.model.walls())
    {
        const auto polygon = to_view_polygon(wp.second);
        if (is_point_in_polygon(p, polygon))
        {
            return wp.first;
        }
    }

    return {};
}

ImVec2 gui::floor_view::to_view(const corecad::model::vector2d &p) const
{
    return ImVec2
    {
        static_cast<float>((p.x * _x_scale) + _x_offset),
        static_cast<float>((p.y * _y_scale) + _y_offset)
    };
}

std::vector<ImVec2> gui::floor_view::to_view_polygon(const domain::plan::model::wall &w) const
{
    const auto& ps = _document.model.points().get(w.start);
    const auto& pe = _document.model.points().get(w.end);
    const auto& sl = _document.model.points().get(w.start_left);
    const auto& sr = _document.model.points().get(w.start_right);
    const auto& el = _document.model.points().get(w.end_left);
    const auto& er = _document.model.points().get(w.end_right);

    return std::vector<ImVec2> { to_view(ps), to_view(sl), to_view(el), to_view(pe), to_view(er), to_view(sr) };
}

bool gui::floor_view::is_point_in_polygon(const ImVec2 &point, const std::vector<ImVec2> &polygon) const
{
    assert(polygon.size() >= 3);

    int sign = 0;

    for (size_t i = 0; i < polygon.size(); ++i)
    {
        size_t ni = (i < polygon.size() - 1) ? i+1 : 0;

        const auto& from = polygon[i];
        const auto& to = polygon[ni];

        const auto poly_vec = ImVec2 { to.x - from.x, to.y - from.y };
        const auto point_vec = ImVec2 { point.x - from.x, point.y - from.y };

        const auto product = poly_vec.x * point_vec.y - poly_vec.y * point_vec.x;

        if (product > 0)
        {
            if (sign < 0)
            {
                return false;
            }
            else
            {
                sign = 1;
            }
        }

        if (product < 0)
        {
            if (sign > 0)
            {
                return false;
            }
            else
            {
                sign = -1;
            }
        }
    }

    return true;
}

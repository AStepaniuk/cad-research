#include "walls_view.h"

using namespace gui;
using namespace domain::plan::model;
using namespace corecad::model;

namespace
{
    namespace Styles
    {
        constexpr auto WallLineColor = IM_COL32(192, 192, 192, 255);
        constexpr auto WallLineThickness = 2.0f;

        constexpr auto WallMidLineColor = IM_COL32(96, 96, 96, 255);
        constexpr auto WallMidLineThickness = 1.0f;


        constexpr auto WallHoveredLineColor = IM_COL32(192, 192, 255, 255);
        constexpr auto WallHoveredFillColor = IM_COL32(64, 64, 92, 255);
        constexpr auto WallHoveredLineThickness = 3.0f;
                
        constexpr auto WallHoveredMidLineColor = IM_COL32(96, 96, 128, 255);
        constexpr auto WallHoveredMidLineThickness = 1.0f;


        constexpr auto WallSelectedLineColor = IM_COL32(208, 208, 208, 255);
        constexpr auto WallSelectedFillColor = IM_COL32(48, 48, 48, 255);
        constexpr auto WallSelectedLineThickness = 2.5f;
                
        constexpr auto WallSelectedMidLineColor = IM_COL32(128, 128, 128, 255);
        constexpr auto WallSelectedMidLineThickness = 1.0f;


        constexpr auto HandleSize = 14.0f;
        constexpr auto HandleSize2 = HandleSize * 0.5f;
        constexpr auto HandleColor = IM_COL32(64, 64, 224, 255);
        constexpr auto HandleThickness = 2.0f;
        constexpr auto HandleHoveredColor = IM_COL32(128, 128, 192, 255);
        constexpr auto HandleActiveColor = IM_COL32(156, 156, 224, 255);
    }

    ImVec2 operator-(const ImVec2& arg, float d)
    {
        return ImVec2 { arg.x - d, arg.y - d };
    }

    ImVec2 operator+(const ImVec2& arg, float d)
    {
        return ImVec2 { arg.x + d, arg.y + d };
    }
}

walls_view::walls_view(const doc::document &doc, const coord_translator& t)
    : _document { doc }
    , _translator { t }
{
}

void walls_view::render(ImDrawList* draw_list)
{
    for (const auto& wp : _document.model.walls())
    {
        const auto& w = wp.second;
        const auto polygon = to_view_polygon(w);

        if (_document.selected_walls.contains(w.index))
        {
            const auto polygon = to_view_polygon(w);

            draw_list->AddConvexPolyFilled(polygon.data(), polygon.size(), Styles::WallSelectedFillColor);
            draw_list->AddLine(polygon[1], polygon[2], Styles::WallSelectedLineColor, Styles::WallSelectedLineThickness);
            draw_list->AddLine(polygon[4], polygon[5], Styles::WallSelectedLineColor, Styles::WallSelectedLineThickness);
            if (w.start_joints == 0)
            {
                draw_list->AddLine(polygon[1], polygon[5], Styles::WallSelectedLineColor, Styles::WallSelectedLineThickness);
            }
            if (w.end_joints == 0)
            {
                draw_list->AddLine(polygon[2], polygon[4], Styles::WallSelectedLineColor, Styles::WallSelectedLineThickness);
            }

            draw_list->AddLine(polygon[0], polygon[3], Styles::WallSelectedMidLineColor, Styles::WallSelectedMidLineThickness);
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

    // hovered wall
    if (_document.hovered_wall_id)
    {
        const auto& w = _document.model.walls().get(_document.hovered_wall_id.value());
        const auto polygon = to_view_polygon(w);

        draw_list->AddConvexPolyFilled(polygon.data(), polygon.size(), Styles::WallHoveredFillColor);
        draw_list->AddLine(polygon[1], polygon[2], Styles::WallHoveredLineColor, Styles::WallHoveredLineThickness);
        draw_list->AddLine(polygon[4], polygon[5], Styles::WallHoveredLineColor, Styles::WallHoveredLineThickness);
        if (w.start_joints == 0)
        {
            draw_list->AddLine(polygon[1], polygon[5], Styles::WallHoveredLineColor, Styles::WallHoveredLineThickness);
        }
        if (w.end_joints == 0)
        {
            draw_list->AddLine(polygon[2], polygon[4], Styles::WallHoveredLineColor, Styles::WallHoveredLineThickness);
        }

        draw_list->AddLine(polygon[0], polygon[3], Styles::WallHoveredMidLineColor, Styles::WallHoveredMidLineThickness);
    }

    // handles
    for (auto wi : _document.selected_walls)
    {
        const auto& w = _document.model.walls().get(wi);

        const auto s = _translator.to_view(w.start);
        const auto e = _translator.to_view(w.end);

        if (_document.hovered_handles.contains(w.start))
        {
            draw_list->AddRect(s - Styles::HandleSize2, s + Styles::HandleSize2, Styles::HandleHoveredColor, 0.0f, 0, Styles::HandleThickness);
        }
        else if (_document.active_handles.contains(w.start))
        {
            draw_list->AddRect(s - Styles::HandleSize2, s + Styles::HandleSize2, Styles::HandleActiveColor, 0.0f, 0, Styles::HandleThickness);
        }
        else
        {
            draw_list->AddRect(s - Styles::HandleSize2, s + Styles::HandleSize2, Styles::HandleColor, 0.0f, 0, Styles::HandleThickness);
        }

        if (_document.hovered_handles.contains(w.end))
        {
            draw_list->AddRect(e - Styles::HandleSize2, e + Styles::HandleSize2, Styles::HandleHoveredColor, 0.0f, 0, Styles::HandleThickness);
        }
        else if (_document.active_handles.contains(w.end))
        {
            draw_list->AddRect(e - Styles::HandleSize2, e + Styles::HandleSize2, Styles::HandleActiveColor, 0.0f, 0, Styles::HandleThickness);
        }
        else
        {
            draw_list->AddRect(e - Styles::HandleSize2, e + Styles::HandleSize2, Styles::HandleColor, 0.0f, 0, Styles::HandleThickness);
        }
    }
}

std::optional<wall::index_t> walls_view::get_wall(float screen_x, float screen_y) const
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

std::vector<vector2d::index_t> walls_view::get_handles(float screen_x, float screen_y) const
{
    std::vector<vector2d::index_t> result;

    auto check_point = [&](vector2d::index_t pi) {
        const auto p = _translator.to_view(pi);

        const auto tl = p - Styles::HandleSize2;
        const auto br = p + Styles::HandleSize2;
        if (tl.x <= screen_x && br.x >= screen_x && tl.y <= screen_y && br.y >= screen_y)
        {
            if (!std::ranges::contains(result, pi))
            {
                result.push_back(pi);
            }
        }
     };

    for (auto wi : _document.selected_walls)
    {
        const auto& w = _document.model.walls().get(wi);
        check_point(w.start);
        check_point(w.end);
   }

   return result;
}

std::vector<ImVec2> walls_view::to_view_polygon(const domain::plan::model::wall &w) const
{
    return std::vector<ImVec2>
    {
        _translator.to_view(w.start),
        _translator.to_view(w.start_left),
        _translator.to_view(w.end_left),
        _translator.to_view(w.end),
        _translator.to_view(w.end_right),
        _translator.to_view(w.start_right) 
    };
}

bool walls_view::is_point_in_polygon(const ImVec2 &point, const std::vector<ImVec2> &polygon) const
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

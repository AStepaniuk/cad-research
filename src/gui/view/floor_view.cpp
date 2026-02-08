#include "floor_view.h"

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

    constexpr float MinScale = 0.0001;
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
    if (val >= MinScale)
    {
        _x_scale = val;
    }
    else
    {
        _x_scale = MinScale;
    }
}

float gui::floor_view::y_scale() const
{
    return _y_scale;
}

void gui::floor_view::y_scale(float val)
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

void gui::floor_view::render()
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

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

        const auto& sp = _document.model.points().get(w.start);
        const auto& ep = _document.model.points().get(w.end);
        const auto s = to_view(sp);
        const auto e = to_view(ep);

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

vector2d floor_view::to_model(float screen_x, float screen_y) const
{
    return vector2d
    {
        (static_cast<double>(screen_x) - _x_offset) / _x_scale,
        (static_cast<double>(screen_y) - _y_offset) / _y_scale
    };
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

std::vector<vector2d::index_t> gui::floor_view::get_handles(float screen_x, float screen_y) const
{
    std::vector<vector2d::index_t> result;

    auto check_point = [&](vector2d::index_t pi) {
        const auto& v = _document.model.points().get(pi);
        const auto p = to_view(v);

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

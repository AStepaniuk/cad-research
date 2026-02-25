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
}

gui::floor_view::floor_view(const doc::document &doc)
    : _document { doc }
    , _translator { _document.model.points() }
    , _walls_view { _document, _translator }
    , _constraints_view{ _document, _translator }
{
}

int floor_view::x_offset() const
{
    return _translator.x_offset();
}

void gui::floor_view::x_offset(int val)
{
    _translator.x_offset(val);
}

int gui::floor_view::y_offset() const
{
    return _translator.y_offset();
}

void gui::floor_view::y_offset(int val)
{
    _translator.y_offset(val);
}

float gui::floor_view::x_scale() const
{
    return _translator.x_scale();
}

void gui::floor_view::x_scale(float val)
{
    _translator.x_scale(val);
}

float gui::floor_view::y_scale() const
{
    return _translator.y_scale();
}

void gui::floor_view::y_scale(float val)
{
    _translator.y_scale(val);
}

void gui::floor_view::render()
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    _walls_view.render(draw_list);
    _constraints_view.render(draw_list);

}

vector2d floor_view::to_model(float screen_x, float screen_y) const
{
    return _translator.to_model({ screen_x, screen_y });
}

corecad::model::vector2d gui::floor_view::model_interaction_tolerance() const
{
    return _translator.to_model({ Styles::HandleSize2, Styles::HandleSize2 });
}

std::optional<wall::index_t> gui::floor_view::get_wall(float screen_x, float screen_y) const
{
    return _walls_view.get_wall(screen_x, screen_y);
}

std::vector<vector2d::index_t> gui::floor_view::get_handles(float screen_x, float screen_y) const
{
    return _walls_view.get_handles(screen_x, screen_y);
}

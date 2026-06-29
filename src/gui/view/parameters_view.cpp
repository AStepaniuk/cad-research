#include "parameters_view.h"

#include <variant>
#include <cmath>

using namespace gui;
using namespace domain::plan::model::shape;
using namespace domain::plan::model::parameter;
using namespace domain::plan::resolver;

namespace
{
    namespace Styles
    {
        constexpr auto CColor = IM_COL32(48, 196, 48, 255);
        constexpr auto CSnapColor = IM_COL32(32, 128, 32, 255);
        constexpr auto CLineThickness = 1.0f;

        constexpr auto FixedCOffset = 20.0f;
        constexpr auto FixedCRadius = 3.0f;
    }

    void draw_text_vertical(ImDrawList* draw_list, const char* text, ImVec2 pos, ImU32 col)
    {
        // Calculate rotation math
        float s = -1.0f;
        float c = 0.0f;
        ImVec2 center = pos; // Rotate around the start point

        auto text_size = ImGui::CalcTextSize(text);

        // Start rotation
        int vtx_start = draw_list->VtxBuffer.Size;
        draw_list->AddText({ pos.x - text_size.x*0.5f, pos.y - text_size.y }, col, text);
        int vtx_end = draw_list->VtxBuffer.Size;

        // Apply transformation to the vertices just added
        for (int i = vtx_start; i < vtx_end; ++i)
        {
            ImDrawVert& v = draw_list->VtxBuffer[i];
            ImVec2 rel = { v.pos.x - center.x, v.pos.y - center.y };
            v.pos.x = center.x + (rel.x * c - rel.y * s);
            v.pos.y = center.y + (rel.x * s + rel.y * c);
        }
    }
}

parameters_view::parameters_view(const doc::document &doc, const translator_t &t, point_resolver& pr)
    : _document { doc }
    , _translator { t }
    , _point_resolver { pr }
{
}

void parameters_view::render(ImDrawList *draw_list)
{
    for (const auto& p : _document.model.data().items<parameter>())
    {
        draw_parameter(draw_list, p.second, Styles::CColor);
    }
}

void parameters_view::draw_parameter(ImDrawList* draw_list, const parameter &p, ImU32 color)
{
    const auto from = std::get<wall_border_point::index_t>(_point_resolver.resolve(p.from));
    const auto to = std::get<wall_border_point::index_t>(_point_resolver.resolve(p.to));

    ImVec2 pf = _translator.to_view(from);
    ImVec2 pt = _translator.to_view(to);

    if (p.value == 0.0)
    {
        if (p.direction == distance_direction::horizontal)
        {
            auto min_y = pf.y < pt.y ? pf.y : pt.y;
            auto max_y = pf.y < pt.y ? pt.y : pf.y;
            draw_list->AddLine({ pf.x - 3, min_y + 10 }, { pf.x - 3, max_y - 10 }, color, Styles::CLineThickness);
        }
        else if (p.direction == distance_direction::vertical)
        {
            auto min_x = pf.x < pt.x ? pf.x : pt.x;
            auto max_x = pf.x < pt.x ? pt.x : pf.x;
            draw_list->AddLine({ min_x + 10, pf.y - 3 }, { max_x - 10, pf.y - 3 }, color, Styles::CLineThickness);
        }
        else
        {
            std::cerr << "diagonal offset directions are not yet dupported" << std::endl;
        }
    }
    else
    {
        if (p.direction == distance_direction::horizontal)
        {
            auto dx = pt.x - pf.x;
            auto baseline = dx * 0.1f;

            auto by = std::max(pf.y, pt.y) + baseline;

            draw_list->AddLine(pf, { pf.x, by + 5 }, color, Styles::CLineThickness);
            draw_list->AddLine(pt, { pt.x, by + 5 }, color, Styles::CLineThickness);
            draw_list->AddLine({ pf.x, by }, { pt.x, by }, color, Styles::CLineThickness);

            draw_list->AddLine({ pf.x - 5, by - 5 }, { pf.x + 5, by + 5 }, color, Styles::CLineThickness);
            draw_list->AddLine({ pt.x - 5, by - 5 }, { pt.x + 5, by + 5 }, color, Styles::CLineThickness);
        
            const auto text = std::format("{:.0f}", std::abs(p.value));

            auto text_size = ImGui::CalcTextSize(text.c_str());
            draw_list->AddText({ (pf.x + pt.x)*0.5f - text_size.x*0.5f, by - text_size.y }, color, text.c_str());
        }
        else  if (p.direction == distance_direction::vertical)
        {
            auto dy = pt.y - pf.y;
            auto baseline = dy * 0.1f;

            auto bx = std::max(pf.x, pt.x) + baseline;

            draw_list->AddLine(pf, { bx + 5, pf.y }, color, Styles::CLineThickness);
            draw_list->AddLine(pt, { bx + 5, pt.y }, color, Styles::CLineThickness);
            draw_list->AddLine({ bx, pf.y }, { bx, pt.y }, color, Styles::CLineThickness);

            draw_list->AddLine({ bx - 5, pf.y - 5 }, { bx + 5, pf.y + 5 }, color, Styles::CLineThickness);
            draw_list->AddLine({ bx - 5, pt.y - 5 }, { bx + 5, pt.y + 5 }, color, Styles::CLineThickness);
        
            const auto text = std::format("{:.0f}", std::abs(p.value));
            draw_text_vertical(draw_list, text.c_str(), { bx, (pt.y + pf.y)*0.5f }, color);
        }
        else
        {
            std::cerr << "diagonal offset directions are not yet dupported" << std::endl;
        }
    }
}

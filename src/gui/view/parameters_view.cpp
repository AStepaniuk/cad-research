#include "parameters_view.h"

#include <variant>
#include <cmath>

#include "overloaded.h"

using namespace gui;
using namespace domain::plan::model::shape;
using namespace domain::plan::model::parameter;
using namespace corecad::model;

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

parameters_view::parameters_view(const doc::document &doc, const translator_t &t)
    : _document { doc }
    , _translator { t }
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
    std::visit(corecad::util::overloaded
        {
            [&](const parameter::concrete_t<distance>& d) {
                ImVec2 pf = to_view(d.from);
                ImVec2 pt = to_view(d.to);

                if (d.value == 0.0)
                {
                    if (d.direction == coordinate2d::x)
                    {
                        auto min_y = pf.y < pt.y ? pf.y : pt.y;
                        auto max_y = pf.y < pt.y ? pt.y : pf.y;
                        draw_list->AddLine({ pf.x - 3, min_y + 10 }, { pf.x - 3, max_y - 10 }, color, Styles::CLineThickness);
                    }
                    else
                    {
                        auto min_x = pf.x < pt.x ? pf.x : pt.x;
                        auto max_x = pf.x < pt.x ? pt.x : pf.x;
                        draw_list->AddLine({ min_x + 10, pf.y - 3 }, { max_x - 10, pf.y - 3 }, color, Styles::CLineThickness);
                    }
                }
                else
                {
                    if (d.direction == coordinate2d::x)
                    {
                        auto dx = pt.x - pf.x;
                        auto baseline = dx * 0.1f;

                        auto by = std::max(pf.y, pt.y) + baseline;

                        draw_list->AddLine(pf, { pf.x, by + 5 }, color, Styles::CLineThickness);
                        draw_list->AddLine(pt, { pt.x, by + 5 }, color, Styles::CLineThickness);
                        draw_list->AddLine({ pf.x, by }, { pt.x, by }, color, Styles::CLineThickness);

                        draw_list->AddLine({ pf.x - 5, by - 5 }, { pf.x + 5, by + 5 }, color, Styles::CLineThickness);
                        draw_list->AddLine({ pt.x - 5, by - 5 }, { pt.x + 5, by + 5 }, color, Styles::CLineThickness);
                    
                        const auto text = std::format("{:.0f}", std::abs(d.value));

                        auto text_size = ImGui::CalcTextSize(text.c_str());
                        draw_list->AddText({ (pf.x + pt.x)*0.5f - text_size.x*0.5f, by - text_size.y }, color, text.c_str());
                    }
                    else
                    {
                        auto dy = pt.y - pf.y;
                        auto baseline = dy * 0.1f;

                        auto bx = std::max(pf.x, pt.x) + baseline;

                        draw_list->AddLine(pf, { bx + 5, pf.y }, color, Styles::CLineThickness);
                        draw_list->AddLine(pt, { bx + 5, pt.y }, color, Styles::CLineThickness);
                        draw_list->AddLine({ bx, pf.y }, { bx, pt.y }, color, Styles::CLineThickness);

                        draw_list->AddLine({ bx - 5, pf.y - 5 }, { bx + 5, pf.y + 5 }, color, Styles::CLineThickness);
                        draw_list->AddLine({ bx - 5, pt.y - 5 }, { bx + 5, pt.y + 5 }, color, Styles::CLineThickness);
                    
                        const auto text = std::format("{:.0f}", std::abs(d.value));
                        draw_text_vertical(draw_list, text.c_str(), { bx, (pt.y + pf.y)*0.5f }, color);
                    }
                }
            },
            [&](const parameter::concrete_t<colinear>& c) {
                ImVec2 p1 = to_view(c.point1);
                ImVec2 p2 = to_view(c.point2);
                ImVec2 p3 = to_view(c.point3);

                const auto dx = p3.x - p1.x;
                const auto dy = p3.y - p1.y;
                const auto hypot = std::hypot(dx, dy);
                if (hypot < 1.0f)
                {
                    // don't draw constraint. Points are too close
                    return;
                }

                const auto ndx = dx/hypot;
                const auto ndy = dy/hypot;

                // rotate normal vector to 90 degrees
                auto rndx = -ndy;
                auto rndy = ndx;

                // make sure rndy is <0
                if (rndy > 0.0f)
                {
                    rndx = -rndx;
                    rndy = -rndy;
                }

                ImVec2 m1 { (p1.x + p2.x) *0.5f, (p1.y + p2.y) *0.5f };
                ImVec2 m2 { (p2.x + p3.x) *0.5f, (p2.y + p3.y) *0.5f };

                ImVec2 ms1 { m1.x + 5.0f*rndx - 20.0f*ndx, m1.y + 5.0f*rndy - 20.0f*ndy };
                ImVec2 me1 { m1.x + 5.0f*rndx + 20.0f*ndx, m1.y + 5.0f*rndy + 20.0f*ndy };
                ImVec2 ms2 { m2.x + 5.0f*rndx - 20.0f*ndx, m2.y + 5.0f*rndy - 20.0f*ndy };
                ImVec2 me2 { m2.x + 5.0f*rndx + 20.0f*ndx, m2.y + 5.0f*rndy + 20.0f*ndy };

                draw_list->AddLine(ms1, me1, Styles::CColor, Styles::CLineThickness);
                draw_list->AddLine(ms2, me2, Styles::CColor, Styles::CLineThickness);
            },
            [&](const parameter::concrete_t<pinned>& pin) {
                ImVec2 p = to_view(pin.point);
                if (pin.coordinate == coordinate2d::x)
                {
                    ImVec2 s { p.x, p.y - Styles::FixedCOffset };
                    ImVec2 e { p.x, p.y + Styles::FixedCOffset };

                    draw_list->AddLine(s, e, color, Styles::CLineThickness);
                    draw_list->AddCircleFilled(p, Styles::FixedCRadius, color);
                }
                else
                {
                    ImVec2 s { p.x - Styles::FixedCOffset, p.y };
                    ImVec2 e { p.x + Styles::FixedCOffset, p.y };

                    draw_list->AddLine(s, e, color, Styles::CLineThickness);
                    draw_list->AddCircleFilled(p, Styles::FixedCRadius, color);
                }
            }
        },
        p.instance
    );
}

ImVec2 parameters_view::to_view(const wall_point_id_t &pid) const
{
    return std::visit([this](const auto& concrete_pid) {
            return _translator.to_view(concrete_pid);
        }
        , pid
    );
}

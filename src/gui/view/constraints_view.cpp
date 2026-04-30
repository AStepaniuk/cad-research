#include "constraints_view.h"

#include <variant>
#include <cmath>

#include "overloaded.h"

using namespace gui;
using namespace corecad::model::constraint;
using namespace domain::plan::model;

namespace
{
    namespace Styles
    {
        constexpr auto CColor = IM_COL32(48, 196, 48, 255);
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

constraints_view::constraints_view(const doc::document &doc, const coord_translator_t &t)
    : _document { doc }
    , _translator { t }
{
}

void constraints_view::render(ImDrawList *draw_list)
{
    for (const auto& c : _document.model.data().items<floor::constraint_t>())
    {
        std::visit(corecad::util::overloaded
            {
                [&](const offset<wall_axis_point, floor::constraint_t>& offs) {
                    ImVec2 pf = _translator.to_view(offs.from);
                    ImVec2 pt = _translator.to_view(offs.to);

                    if (std::abs(offs.distance) < 0.0001)
                    {
                        draw_list->AddLine(pf, pt, Styles::CColor, Styles::CLineThickness);
                    }
                    else
                    {
                        if (offs.direction == offset_direction::horizontal)
                        {
                            auto dx = pt.x - pf.x;
                            auto baseline = dx * 0.1f;

                            auto by = std::max(pf.y, pt.y) + baseline;

                            draw_list->AddLine(pf, { pf.x, by + 5 }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine(pt, { pt.x, by + 5 }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine({ pf.x, by }, { pt.x, by }, Styles::CColor, Styles::CLineThickness);

                            draw_list->AddLine({ pf.x - 5, by - 5 }, { pf.x + 5, by + 5 }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine({ pt.x - 5, by - 5 }, { pt.x + 5, by + 5 }, Styles::CColor, Styles::CLineThickness);
                        
                            const auto text = std::format("{:.0f}", std::abs(offs.distance));

                            auto text_size = ImGui::CalcTextSize(text.c_str());
                            draw_list->AddText({ (pf.x + pt.x)*0.5f - text_size.x*0.5f, by - text_size.y }, Styles::CColor, text.c_str());
                        }
                        else
                        {
                            auto dy = pt.y - pf.y;
                            auto baseline = dy * 0.1f;

                            auto bx = std::max(pf.x, pt.x) + baseline;

                            draw_list->AddLine(pf, { bx + 5, pf.y }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine(pt, { bx + 5, pt.y }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine({ bx, pf.y }, { bx, pt.y }, Styles::CColor, Styles::CLineThickness);

                            draw_list->AddLine({ bx - 5, pf.y - 5 }, { bx + 5, pf.y + 5 }, Styles::CColor, Styles::CLineThickness);
                            draw_list->AddLine({ bx - 5, pt.y - 5 }, { bx + 5, pt.y + 5 }, Styles::CColor, Styles::CLineThickness);
                        
                            const auto text = std::format("{:.0f}", std::abs(offs.distance));
                            draw_text_vertical(draw_list, text.c_str(), { bx, (pt.y + pf.y)*0.5f }, Styles::CColor);
                        }
                    }
                },
                [&](const fixed<wall_axis_point, floor::constraint_t>& fix) {
                    ImVec2 p = _translator.to_view(fix.point);
                    if (fix.coordinate == fixed_coordinate::x)
                    {
                        ImVec2 s { p.x, p.y - Styles::FixedCOffset };
                        ImVec2 e { p.x, p.y + Styles::FixedCOffset };

                        draw_list->AddLine(s, e, Styles::CColor, Styles::CLineThickness);
                        draw_list->AddCircleFilled(p, Styles::FixedCRadius, Styles::CColor);
                    }
                    else
                    {
                        ImVec2 s { p.x - Styles::FixedCOffset, p.y };
                        ImVec2 e { p.x + Styles::FixedCOffset, p.y };

                        draw_list->AddLine(s, e, Styles::CColor, Styles::CLineThickness);
                        draw_list->AddCircleFilled(p, Styles::FixedCRadius, Styles::CColor);
                    }
                },
                [&](const aligned<wall_axis_point, floor::constraint_t>& al) {
                    ImVec2 p1 = _translator.to_view(al.point1);
                    ImVec2 p2 = _translator.to_view(al.point2);
                    ImVec2 p3 = _translator.to_view(al.point3);

                    const auto dx = p3.x - p1.x;
                    const auto dy = p3.y - p1.y;
                    const auto hypot = std::hypot(dx, dy);
                    if (hypot < 1.0f)
                    {
                        // don't draw constraint. Points are too close
                    }

                    const auto ndx = dx/hypot;
                    const auto ndy = dy/hypot;

                    // rotate normal vector to 90 degrees
                    auto rndx = -ndy;
                    auto rndy = ndx;

                    // make sure rndy is >0
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
                }
            },
            c.second.instance
        );
    }
}

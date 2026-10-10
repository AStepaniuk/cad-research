#include "wall_profile_preview_canvas.h"

#include <algorithm>
#include "translate.h"

using namespace gui::localization;

gui::wall_profile_preview_canvas::wall_profile_preview_canvas(context_data ctx)
    : _ctx { ctx }
{
}

void gui::wall_profile_preview_canvas::reset_zoom_view(ImVec2 canvas_size)
{
    double total_thickness_mm = 0.0;
    for (const auto& layer : _ctx.editing_layers)
    {
        total_thickness_mm += layer.thickness.val().numerical_value_in(corecad::model::mm);
    }

    if (total_thickness_mm <= 0.0) total_thickness_mm = 300.0;

    _zoom_factor = (canvas_size.y * 0.70f) / static_cast<float>(total_thickness_mm);    
    _pan_offset = ImVec2(canvas_size.x * 0.5f, canvas_size.y * 0.5f);
}

void gui::wall_profile_preview_canvas::render(const char* str_id, float target_width, float target_height)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImVec2 explicit_size(
        target_width <= 0.0f ? ImGui::GetContentRegionAvail().x : target_width, 
        target_height <= 0.0f ? 50.0f : target_height
    );

    if (ImGui::BeginChild(str_id, explicit_size, true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove))
    {
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        reset_zoom_view(explicit_size);

        // Render Background
        draw_list->AddRectFilled(canvas_pos, ImVec2(canvas_pos.x + explicit_size.x, canvas_pos.y + explicit_size.y), IM_COL32(24, 24, 27, 255));

        ImVec2 origin_screen_pos = ImVec2(canvas_pos.x + _pan_offset.x, canvas_pos.y + _pan_offset.y);

        if (!_ctx.editing_layers.empty())
        {
            double total_thickness_mm = 0.0;
            for (const auto& layer : _ctx.editing_layers)
            {
                total_thickness_mm += layer.thickness.val().numerical_value_in(corecad::model::mm);
            }
            
            float current_y_mm = -static_cast<float>(total_thickness_mm) * 0.5f;
            float wall_length_pixels = explicit_size.x * 0.80f;
            float half_length_x = wall_length_pixels * 0.5f;

            for (size_t i = 0; i < _ctx.editing_layers.size(); ++i)
            {
                const auto& layer = _ctx.editing_layers[i];
                float layer_thick_pixels = static_cast<float>(layer.thickness.val().numerical_value_in(corecad::model::mm)) * _zoom_factor;

                ImVec2 rect_min = ImVec2(origin_screen_pos.x - half_length_x, origin_screen_pos.y + (current_y_mm * _zoom_factor));
                ImVec2 rect_max = ImVec2(origin_screen_pos.x + half_length_x, rect_min.y + layer_thick_pixels);

                ImU32 material_color = IM_COL32(63, 63, 70, 255);
                std::string_view material_label = tr("Unassigned Material");

                if (layer.material.val())
                {
                    const auto& asset = _ctx.materials_lookup.get(layer.material);
                    material_label = asset.standard_name.val();
                }

                draw_list->AddRectFilled(rect_min, rect_max, material_color);                
                draw_list->AddRect(rect_min, rect_max, IM_COL32(9, 9, 11, 255), 0.0f, 0, 1.5f);

                if (layer_thick_pixels > 14.0f) 
                {
                    char format_buf[128];
                    snprintf(format_buf, sizeof(format_buf), tr("%.1f mm - %s").data(), 
                             layer.thickness.val().numerical_value_in(corecad::model::mm), 
                             material_label.data());
                    
                    ImVec2 text_size = ImGui::CalcTextSize(format_buf);
                    
                    if (text_size.x < wall_length_pixels - 10.0f)
                    {
                        ImVec2 text_pos = ImVec2(
                            origin_screen_pos.x - (text_size.x * 0.5f),
                            rect_min.y + (layer_thick_pixels - text_size.y) * 0.5f
                        );
                        
                        draw_list->AddText(ImVec2(text_pos.x + 1.0f, text_pos.y + 1.0f), IM_COL32(0, 0, 0, 240), format_buf);
                        draw_list->AddText(text_pos, IM_COL32(244, 244, 245, 255), format_buf);
                    }
                }

                current_y_mm += static_cast<float>(layer.thickness.val().numerical_value_in(corecad::model::mm));
            }
        }
        else
        {
            std::string_view empty_msg = tr("No Layers Defined to Render Preview Profile");
            ImVec2 txt_sz = ImGui::CalcTextSize(empty_msg.data());
            draw_list->AddText(
                ImVec2(canvas_pos.x + (explicit_size.x - txt_sz.x) * 0.5f, canvas_pos.y + (explicit_size.y - txt_sz.y) * 0.5f),
                IM_COL32(161, 161, 170, 255),
                empty_msg.data()
            );
        }
    }
    ImGui::EndChild();
    
    ImGui::PopStyleVar();
}

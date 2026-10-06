#pragma once

#include <imgui.h>

#include "property.h"
#include "units.h"

namespace gui::components
{
    template <typename TModel>
    class length_cell
    {
    public:
        using prop_ptr_t = corecad::model::property<corecad::model::length_mm_t, TModel> TModel::*;

        length_cell(prop_ptr_t prop_ptr, corecad::model::length_mm_t step)
            : _prop_ptr { prop_ptr }
            , _step { step.numerical_value_in(corecad::model::mm) }
            , _step_fast { _step * 10 }
        {
        }

        bool render(TModel& item)
        {
            auto& prop = item.*_prop_ptr;
            double value = prop.val().numerical_value_in(corecad::model::mm);
            
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::InputScalar("##QtyInput", ImGuiDataType_Double, &value, &_step, &_step_fast, "%.1f"))
            {
                if (value < 0.0) value = 0.0;                
                prop = value * corecad::model::mm;
            }

            return ImGui::IsItemDeactivatedAfterEdit();
        }

    private:
        prop_ptr_t _prop_ptr;
        double _step;
        double _step_fast;
    };
}

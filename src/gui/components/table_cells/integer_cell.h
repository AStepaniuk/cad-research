#pragma once

#include <imgui.h>
#include <algorithm>

#include "property.h"

namespace gui::components
{
    template <typename TModel>
    class integer_cell 
    {
    public:
        using prop_ptr_t = corecad::model::property<int, TModel> TModel::*;

        integer_cell(prop_ptr_t prop_ptr, int step, int min_v, int max_v)
            : _prop_ptr { prop_ptr }
            , _step { step }
            , _step_fast { step * 10 }
            , _min_v { min_v }
            , _max_v { max_v }
        {
        }

        bool render(TModel& item)
        {
            auto& prop = item.*_prop_ptr;
            int value = prop.val();
            
            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::InputInt("##IntInput", &value, _step, _step_fast))
            {
                value = std::clamp(value, _min_v, _max_v);                
                prop = value;
            }

            return ImGui::IsItemDeactivatedAfterEdit();
        }

    private:
        prop_ptr_t _prop_ptr;
        int _step;
        int _step_fast;
        int _min_v;
        int _max_v;
    };
}

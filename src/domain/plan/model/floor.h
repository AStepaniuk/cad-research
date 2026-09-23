#pragma once

#include "registry_pool.h"

#include "constraint.h"
#include "fixed.h"
#include "offset.h"
#include "aligned.h"
#include "wall.h"
#include "wall_junction.h"
#include "parameter.h"
#include "history/history.h"

namespace domain::plan::model
{
    class floor
    {
    public:
        struct constraint_data
        {
            parameter::parameter::index_t source_parameter_index;
        };

        using constraint_t = corecad::model::constraint::constraint<shape::wall_points_ids_tl>;

        using data_t = corecad::model::registry_pool<
            std::pair<constraint_t, constraint_data>,
            std::pair<shape::wall_axis_point, shape::wall_axis_point_data>,
            shape::wall_axis_line,
            std::pair<shape::wall_border_point, shape::wall_border_point_data>,
            shape::wall_border_line,
            shape::wall_material_definition,
            shape::wall_layer,
            shape::wall_compound_type,
            shape::wall,
            shape::wall_junction,
            parameter::parameter
        >;

        using history_t = corecad::model::history::history<
            data_t,
            shape::wall_axis_point,
            shape::wall_axis_line,
            shape::wall_material_definition,
            shape::wall_layer,
            shape::wall_compound_type,
            shape::wall,
            shape::wall_junction,
            parameter::parameter
        >;

        const data_t& data() const;
        data_t& data();

        history_t& history();
    
    private:
        data_t _data;
        history_t _history { _data };
    };
}

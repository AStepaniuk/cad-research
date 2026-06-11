#pragma once

#include "registry_pool.h"

#include "constraint.h"
#include "fixed.h"
#include "offset.h"
#include "aligned.h"
#include "wall.h"
#include "parameter.h"
#include "history/history.h"

namespace domain::plan::model
{
    class floor
    {
    public:
        template <typename TModel>
        using fixed_wall_axis_point_t = corecad::model::constraint::fixed<shape::wall_axis_point, TModel>;

        template <typename TModel>
        using offset_wall_axis_point_t = corecad::model::constraint::offset<shape::wall_axis_point, TModel>;

        template <typename TModel>
        using aligned_wall_axis_point_t = corecad::model::constraint::aligned<shape::wall_axis_point, TModel>;

        using constraint_t = corecad::model::constraint::constraint<
            fixed_wall_axis_point_t,
            offset_wall_axis_point_t,
            aligned_wall_axis_point_t
        >;

        using data_t = corecad::model::registry_pool<
            constraint_t,
            shape::wall_axis_point,
            shape::wall_axis_line,
            shape::wall_border_point,
            shape::wall_border_line,
            shape::wall,
            parameter::parameter
        >;

        using history_t = corecad::model::history::history<
            data_t,
            constraint_t,
            shape::wall_axis_point,
            shape::wall_axis_line,
            shape::wall,
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

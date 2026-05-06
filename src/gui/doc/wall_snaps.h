#pragma once

#include <unordered_map>
#include <vector>

#include "floor.h"

namespace gui { namespace doc {
    class wall_snaps
    {
        const corecad::model::registry<domain::plan::model::wall_axis_point>& _wall_axis_points;

        corecad::model::registry<domain::plan::model::floor::constraint_t> _constraints;

        using rank_data = std::pair<domain::plan::model::floor::constraint_t::index_t, double>;
        std::vector<rank_data> _ranks;

        std::unordered_map<
            domain::plan::model::floor::constraint_t::index_t,
            std::vector<domain::plan::model::wall_axis_point::index_t>
        > _affected_points;

        struct affected_point_data
        {
            size_t refcount;
            domain::plan::model::floor::constraint_t::index_t v_fix;
            domain::plan::model::floor::constraint_t::index_t h_fix;
        };
        
        std::unordered_map<
            domain::plan::model::wall_axis_point::index_t,
            affected_point_data
        > _affected_points_data;

    public:
        wall_snaps(const corecad::model::registry<domain::plan::model::wall_axis_point>& wall_axis_points);

        void clear();
        void add(
            domain::plan::model::floor::constraint_t&& c,
            double rank,
            domain::plan::model::wall_axis_point::index_t affected_point
        );
        void ease();

        const corecad::model::registry<domain::plan::model::floor::constraint_t>& constraints() const;
        void clone_active_handle_constraints(corecad::model::registry<domain::plan::model::floor::constraint_t>& dest) const;
    };
}}

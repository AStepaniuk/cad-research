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
        
        template<typename... TPoints>
            requires (std::convertible_to<TPoints, domain::plan::model::wall_axis_point::index_t> && ...)
        void add(domain::plan::model::floor::constraint_t&& c, double rank, TPoints... affected_points)
        {
            using namespace domain::plan::model;
            using namespace corecad::model::constraint;
            
            const auto cid = _constraints.put(std::move(c));
            _ranks.push_back({cid, rank});

            _affected_points[cid] = { static_cast<wall_axis_point::index_t>(affected_points)... };

            auto process_point = [&](wall_axis_point::index_t p_idx) {
                auto [it, inserted] = _affected_points_data.try_emplace(p_idx);
                
                if (inserted) {
                    it->second.refcount = 1;
                    const auto& p_data = _wall_axis_points.get(p_idx);

                    // Create the anchor fixes for the affected point
                    it->second.h_fix = _constraints.put(
                        floor::constraint_t::create<floor::fixed_wall_axis_point_t>(p_idx, p_data.x, fixed_coordinate::x)
                    );
                    it->second.v_fix = _constraints.put(
                        floor::constraint_t::create<floor::fixed_wall_axis_point_t>(p_idx, p_data.y, fixed_coordinate::y)
                    );
                } else {
                    it->second.refcount++;
                }
            };

            (process_point(affected_points), ...);
        }

        void ease();

        const corecad::model::registry<domain::plan::model::floor::constraint_t>& constraints() const;
        void clone_active_handle_constraints(corecad::model::registry<domain::plan::model::floor::constraint_t>& dest) const;
    };
}}

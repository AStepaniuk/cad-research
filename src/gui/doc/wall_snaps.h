#pragma once

#include <unordered_map>
#include <vector>

#include "floor.h"

namespace gui::doc {
    class wall_snaps
    {
        using wall_axis_point = domain::plan::model::shape::wall_axis_point;
        using parameter_t = domain::plan::model::parameter::parameter;
        using constraint_t = domain::plan::model::floor::constraint_t;


        const corecad::model::registry<wall_axis_point>& _wall_axis_points;

        corecad::model::registry<parameter_t> _parameters;
        corecad::model::registry<constraint_t> _anchors;

        using rank_data = std::pair<parameter_t::index_t, double>;
        std::vector<rank_data> _ranks;

        std::unordered_map<
            parameter_t::index_t,
            std::vector<wall_axis_point::index_t>
        > _affected_points;

        struct affected_point_data
        {
            size_t refcount;
            constraint_t::index_t v_fix;
            constraint_t::index_t h_fix;
        };
        
        std::unordered_map<
            wall_axis_point::index_t,
            affected_point_data
        > _affected_points_data;

    public:
        wall_snaps(const corecad::model::registry<wall_axis_point>& wall_axis_points);

        void clear();
        
        template<typename... TPoints>
            requires (std::convertible_to<TPoints, wall_axis_point::index_t> && ...)
        void add(parameter_t p, double rank, TPoints... affected_points)
        {
            using namespace domain::plan::model;
            using namespace corecad::model::constraint;
            
            const auto pid = _parameters.put(std::move(p));
            _ranks.push_back({pid, rank});

            _affected_points[pid] = { static_cast<wall_axis_point::index_t>(affected_points)... };

            auto process_point = [&](wall_axis_point::index_t p_idx) {
                auto [it, inserted] = _affected_points_data.try_emplace(p_idx);
                
                if (inserted)
                {
                    it->second.refcount = 1;
                    const auto& p_data = _wall_axis_points.get(p_idx);

                    // Create the anchor fixes for the affected point
                    it->second.h_fix = _anchors.put(
                        constraint_t::create<fixed>(p_idx, p_data.x, corecad::model::coordinate2d::x)
                    );
                    it->second.v_fix = _anchors.put(
                        constraint_t::create<fixed>(p_idx, p_data.y, corecad::model::coordinate2d::y)
                    );
                }
                else
                {
                    it->second.refcount++;
                }
            };

            (process_point(affected_points), ...);
        }

        void ease();

        const corecad::model::registry<parameter_t>& parameters() const;
        const corecad::model::registry<constraint_t>& anchors() const;
    };
}

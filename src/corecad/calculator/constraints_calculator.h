#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>

#include <GCS.h>

#include "overloaded.h"
#include "constraint.h"
#include "vector2d.h"
#include "registry.h"

namespace corecad { namespace calculator
{
    template<corecad::model::IsVector2D TVector, typename TConstraintModel>
    class constraints_calculator
    {
    public:
        void recalculate_all(
            const domain::plan::model::floor::constraints_t& parameters,        
            model::registry<TVector>& points
        )
        {
            m_sys.clear();

            std::vector<GCS::Point> gcs_points;
            std::vector<double> gcs_params;
            std::unordered_set<size_t> gcs_constants;

            gcs_points.resize(points.size());
            gcs_params.resize(points.size() * 2 + parameters.size());
            size_t next_idx = 0;
            size_t next_p = 0;

            std::unordered_map<typename TVector::index_t, GCS::Point*> gcs_points_table;

            auto get_or_add_gcs_point = [&](TVector::index_t i) {
                auto [iter, inserted] = gcs_points_table.try_emplace(i, nullptr);

                if (inserted)
                {
                    const TVector& p = points.get(i);
                    GCS::Point& gcs_p = gcs_points[next_idx];
                    gcs_p.x = &(gcs_params[next_p]);
                    gcs_p.y = &(gcs_params[next_p + 1]);

                    *(gcs_p.x) = p.x;
                    *(gcs_p.y) = p.y;

                    iter->second = &gcs_p;

                    next_idx++;
                    next_p += 2;
                }

                return iter->second;
            };

            for(const auto& c : parameters)
            {
                std::visit(util::overloaded
                    {
                        [&](const model::constraint::offset<TVector, TConstraintModel>& offs) {
                            auto f_gcs_p = get_or_add_gcs_point(offs.from);
                            auto t_gcs_p = get_or_add_gcs_point(offs.to);

                            gcs_params[next_p] = offs.distance;

                            if (offs.direction == model::constraint::offset_direction::horizontal)
                            {
                                m_sys.addConstraintDifference(f_gcs_p->x, t_gcs_p->x, &gcs_params[next_p]);
                            }
                            else
                            {
                                m_sys.addConstraintDifference(f_gcs_p->y, t_gcs_p->y, &gcs_params[next_p]);
                            }

                            next_p++;
                        },
                        [&](const model::constraint::fixed<TVector, TConstraintModel>& fix) {
                            auto gcs_p = get_or_add_gcs_point(fix.point);

                            if (fix.coordinate == model::constraint::fixed_coordinate::x)
                            {
                                *(gcs_p->x) = fix.value;
                                gcs_constants.emplace(gcs_p->x - gcs_params.data());
                            }
                            else
                            {
                                *(gcs_p->y) = fix.value;
                                gcs_constants.emplace(gcs_p->y - gcs_params.data());
                            }
                        }
                    },
                    c.second.instance
                );
            }

            auto gcs_variables = gcs_points
                | std::views::take(next_idx)
                | std::views::transform([] (const GCS::Point& p) { return std::array { p.x, p.y }; })
                | std::views::join
                | std::views::filter([&](double* d) { 
                    return !gcs_constants.contains(d - gcs_params.data());
                })
                | std::ranges::to<std::vector>();

            auto res = m_sys.solve(gcs_variables);
            if (!res == GCS::SolveStatus::Success)
            {
                // TODO: handle errors
                std::cout << "Solve failed: " << res << std::endl;
            }
            else
            {
                m_sys.applySolution();

                for(const auto& pair : gcs_points_table)
                {
                    auto& p = points.get(pair.first);
                    p.x = *(pair.second->x);
                    p.y = *(pair.second->y);
                }
            }
        }

    private:
        GCS::System m_sys;
    };
}}

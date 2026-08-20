#pragma once

#include <flat_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <concepts>

#include <GCS.h>
#include "ConstraintP2LOnLeft.h"
#include "ConstraintParallel2.h"

#include "overloaded.h"
#include "constraint.h"
#include "vector2d.h"
#include "registry.h"

namespace corecad::calculator
{
    enum class constraint_calculation_result { success, failed };

    template <typename TConstraintModel, typename TRegistryPool>
    class constraints_calculator;

    template <template<typename> typename TConstraintModel, typename TRegistryPool, typename... TVectorIndex>
    requires (
        model::constraint::IsConstraint<TConstraintModel<corecad::util::type_list<TVectorIndex...>>>
        && (model::IsVector2D<typename TVectorIndex::tag_t> && ...)
    )
    class constraints_calculator<TConstraintModel<corecad::util::type_list<TVectorIndex...>>, TRegistryPool>
    {
    public:
        using constraint_t = TConstraintModel<corecad::util::type_list<TVectorIndex...>>;

        constraints_calculator(TRegistryPool& data)
            : _data { data }
        {}

        constraint_calculation_result recalculate_all()
        {
            return recalculate_all(_data.template items<constraint_t>() | std::views::values);
        }

        template <typename R>
        requires
            std::ranges::input_range<R> && 
            std::same_as<std::ranges::range_value_t<R>, constraint_t>
        constraint_calculation_result recalculate_all(R&& iterable)
        {
            m_sys.clear();

            std::vector<GCS::Point> gcs_points;
            std::vector<double> gcs_params;
            std::unordered_set<size_t> gcs_constants;

            auto total_points_size = (_data.template size<typename TVectorIndex::tag_t>() + ...);

            gcs_points.resize(total_points_size);
            gcs_params.resize(total_points_size * 2);
            size_t next_idx = 0;
            size_t next_p = 0;

            using point_id_t = typename constraint_t::point_id_t;

            std::flat_map<point_id_t, GCS::Point*> gcs_points_table;

            auto get_or_add_gcs_point = [&](const point_id_t& i) {
                auto [iter, inserted] = gcs_points_table.try_emplace(i, nullptr);
                if (inserted)
                {
                    GCS::Point& gcs_p = gcs_points[next_idx];
                    gcs_p.x = &(gcs_params[next_p]);
                    gcs_p.y = &(gcs_params[next_p + 1]);

                    std::visit([&] (auto p_id) {
                        using vector2d_t = decltype(p_id)::tag_t;
                        const auto& p = _data.get(p_id);

                        *(gcs_p.x) = p.x;
                        *(gcs_p.y) = p.y;
                    } , i);

                    iter->second = &gcs_p;

                    next_idx++;
                    next_p += 2;
                }

                return iter->second;
            };

            for(const auto& c : iterable)
            {
                std::visit(util::overloaded
                    {
                        [&](const typename constraint_t::template concrete_t<model::constraint::offset>& offs) {
                            auto f_gcs_p = get_or_add_gcs_point(offs.from);
                            auto t_gcs_p = get_or_add_gcs_point(offs.to);

                            if (offs.direction == model::coordinate2d::x)
                            {
                                m_sys.addConstraintDifference(f_gcs_p->x, t_gcs_p->x, &(const_cast<double&>(offs.distance.val())));
                            }
                            else
                            {
                                m_sys.addConstraintDifference(f_gcs_p->y, t_gcs_p->y, &(const_cast<double&>(offs.distance.val())));
                            }
                        },
                        [&](const typename constraint_t::template concrete_t<model::constraint::fixed>& fix) {
                            auto gcs_p = get_or_add_gcs_point(fix.point);

                            if (fix.coordinate == model::coordinate2d::x)
                            {
                                *(gcs_p->x) = fix.value;
                                gcs_constants.emplace(gcs_p->x - gcs_params.data());
                            }
                            else
                            {
                                *(gcs_p->y) = fix.value;
                                gcs_constants.emplace(gcs_p->y - gcs_params.data());
                            }
                        },
                        [&](const typename constraint_t::template concrete_t<model::constraint::aligned>& al) {
                            auto gcs_p1 = get_or_add_gcs_point(al.point1);
                            auto gcs_p2 = get_or_add_gcs_point(al.point2);
                            auto gcs_p3 = get_or_add_gcs_point(al.point3);

                            m_sys.addConstraintPointOnLine(*gcs_p1, *gcs_p2, *gcs_p3);
                        },
                        [&](const typename constraint_t::template concrete_t<model::constraint::parallel_distant>& pd) {
                            auto gcs_l1s = get_or_add_gcs_point(pd.line1_start);
                            auto gcs_l1e = get_or_add_gcs_point(pd.line1_end);
                            auto gcs_l2s = get_or_add_gcs_point(pd.line2_start);
                            auto gcs_l2e = get_or_add_gcs_point(pd.line2_end);

                            auto* dist_constraint = new GCS::ConstraintP2LOnLeft(
                                *gcs_l1s, *gcs_l1e, *gcs_l2s,
                                &(const_cast<double&>(pd.distance.val()))
                            );
                            m_sys.addConstraint(dist_constraint);

                            auto* parallel_constraint = new GCS::ConstraintParallel2(*gcs_l1s, *gcs_l1e, *gcs_l2s, *gcs_l2e);
                            m_sys.addConstraint(parallel_constraint);
                        }
                    },
                    c.instance
                );
            }

            auto gcs_variables = gcs_params 
                | std::views::take(next_idx * 2)
                | std::views::transform([](double& val) { return &val; })
                | std::views::filter([&](double* d) { 
                    return !gcs_constants.contains(d - gcs_params.data());
                })
                | std::ranges::to<std::vector>();

            auto res = m_sys.solve(gcs_variables, true, GCS::Algorithm::LevenbergMarquardt, true);
            if (res != GCS::SolveStatus::Success)
            {
                // TODO: handle errors
                std::cout << "Solve failed: " << res << std::endl;

                return constraint_calculation_result::failed;
            }
            else
            {
                m_sys.applySolution();

                for(const auto& pair : gcs_points_table)
                {
                    std::visit([&] (auto p_id) {
                        using vector2d_t = decltype(p_id)::tag_t;
                        auto& p = _data.get(p_id);
                        p.x = *(pair.second->x);
                        p.y = *(pair.second->y);
                    } , pair.first);
                }

                return constraint_calculation_result::success;
            }
        }

    private:
        GCS::System m_sys;

        TRegistryPool& _data;
    };
}

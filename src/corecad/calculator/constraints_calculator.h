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
#include "point2d.h"
#include "registry.h"
#include "members_iterator.h"

namespace corecad::calculator
{
    enum class constraint_calculation_result { success, failed };

    template <typename TConstraintModel, typename TRegistryPool>
    class constraints_calculator;

    template <template<typename> typename TConstraintModel, typename TRegistryPool, typename... TVectorIndex>
    requires (
        model::constraint::IsConstraint<TConstraintModel<corecad::util::type_list<TVectorIndex...>>>
        && (model::IsPoint2D<typename TVectorIndex::tag_t> && ...)
    )
    class constraints_calculator<TConstraintModel<corecad::util::type_list<TVectorIndex...>>, TRegistryPool>
    {
    public:
        using constraint_t = TConstraintModel<corecad::util::type_list<TVectorIndex...>>;

    private:
        template <typename TMember>
        using length_property = model::is_property_of_type<TMember, model::length_mm_t>;

        template <typename TVariant>
        struct max_const_parametric_member_count_impl;

        template <typename... Ts>
        struct max_const_parametric_member_count_impl<std::variant<Ts...>>
        {
            static constexpr size_t value = [] {
                size_t counts[] = { meta::count_members<length_property, Ts>()... };
                size_t max_val = 0;
                for (size_t count : counts)
                {
                    if (count > max_val) max_val = count;
                }
                return max_val;
            }();
        };

        static constexpr size_t max_const_parametric_properties_in_constraint =
            max_const_parametric_member_count_impl<typename constraint_t::instance_t>::value;

    public:
        constraints_calculator(TRegistryPool& data)
            : _data { data }
        {}

        constraint_calculation_result recalculate_all()
        {
            return recalculate_all(_data.template items<constraint_t>() | std::views::values);
        }

        template <typename R>
        requires
            std::ranges::forward_range<R> &&
            std::ranges::sized_range<R> &&
            std::same_as<std::ranges::range_value_t<R>, constraint_t>
        constraint_calculation_result recalculate_all(R&& iterable)
        {
            m_sys.clear();

            std::vector<GCS::Point> gcs_points;
            std::vector<double> gcs_params;
            std::unordered_set<size_t> gcs_constants;

            const auto total_points_size = (_data.template size<typename TVectorIndex::tag_t>() + ...);
            const auto points_param_size = total_points_size * 2;
            const auto max_const_param_size = std::ranges::distance(iterable) * max_const_parametric_properties_in_constraint;

            gcs_points.resize(total_points_size);
            gcs_params.resize(points_param_size + max_const_param_size);
            size_t next_point_idx = 0;
            size_t next_const_idx = points_param_size;

            using point_id_t = typename constraint_t::point_id_t;

            std::flat_map<point_id_t, GCS::Point*> gcs_points_table;

            auto get_or_add_gcs_point = [&](const point_id_t& i) {
                auto [iter, inserted] = gcs_points_table.try_emplace(i, nullptr);
                if (inserted)
                {
                    const size_t current_param_idx = next_point_idx * 2;

                    GCS::Point& gcs_p = gcs_points[next_point_idx];
                    gcs_p.x = &(gcs_params[current_param_idx]);
                    gcs_p.y = &(gcs_params[current_param_idx + 1]);

                    std::visit([&] (auto p_id) {
                        const auto& p = _data.get(p_id);

                        *(gcs_p.x) = p.x.val().numerical_value_in(model::mm);
                        *(gcs_p.y) = p.y.val().numerical_value_in(model::mm);
                    } , i);

                    iter->second = &gcs_p;

                    next_point_idx++;
                }

                return iter->second;
            };

            auto add_gcs_const_param = [&](model::length_mm_t l) {
                gcs_params[next_const_idx] = l.numerical_value_in(model::mm);
                const auto res = &gcs_params[next_const_idx];

                next_const_idx++;

                return res;
            };

            for(const auto& c : iterable)
            {
                std::visit(util::overloaded
                    {
                        [&](const typename constraint_t::template concrete_t<model::constraint::offset>& offs) {
                            auto f_gcs_point = get_or_add_gcs_point(offs.from);
                            auto t_gcs_point = get_or_add_gcs_point(offs.to);
                            auto dist_gsc_param = add_gcs_const_param(offs.distance);

                            if (offs.direction == model::coordinate2d::x)
                            {
                                m_sys.addConstraintDifference(f_gcs_point->x, t_gcs_point->x, dist_gsc_param);
                            }
                            else
                            {
                                m_sys.addConstraintDifference(f_gcs_point->y, t_gcs_point->y, dist_gsc_param);
                            }
                        },
                        [&](const typename constraint_t::template concrete_t<model::constraint::fixed>& fix) {
                            auto gcs_p = get_or_add_gcs_point(fix.point);

                            if (fix.coordinate == model::coordinate2d::x)
                            {
                                *(gcs_p->x) = fix.value.val().numerical_value_in(model::mm);
                                gcs_constants.emplace(gcs_p->x - gcs_params.data());
                            }
                            else
                            {
                                *(gcs_p->y) = fix.value.val().numerical_value_in(model::mm);
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
                            auto gsc_dist = add_gcs_const_param(pd.distance);

                            auto* dist_constraint = new GCS::ConstraintP2LOnLeft(*gcs_l1s, *gcs_l1e, *gcs_l2s, gsc_dist);
                            m_sys.addConstraint(dist_constraint);

                            auto* parallel_constraint = new GCS::ConstraintParallel2(*gcs_l1s, *gcs_l1e, *gcs_l2s, *gcs_l2e);
                            m_sys.addConstraint(parallel_constraint);
                        }
                    },
                    c.instance
                );
            }

            auto gcs_variables = gcs_params 
                | std::views::take(next_point_idx * 2)
                | std::views::enumerate
                | std::views::filter([&](const auto& pair) {
                    auto [idx, val] = pair;
                    return !gcs_constants.contains(idx);
                })
                | std::views::transform([&](const auto& pair) { return &gcs_params[std::get<0>(pair)]; })
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
                        p.x = (*(pair.second->x)) * model::mm;
                        p.y = (*(pair.second->y)) * model::mm;
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

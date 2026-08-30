#include "wall_calculator.h"

#include <vector>
#include <map>
#include <algorithm>
#include <ranges>
#include <optional>
#include <cmath>
#include <numbers>
#include <iostream>

using namespace domain::plan::calculator;
using namespace domain::plan::model::shape;
using namespace domain::plan::model;
using namespace domain::plan;
using namespace corecad::model;

namespace
{
    double calculate_line_direction(const wall_axis_line& l, const model::floor& f)
    {
        const auto& ps = f.data().get(l.s);
        const auto& pe = f.data().get(l.e);

        return std::atan2(pe.y - ps.y, pe.x - ps.x);
    }

    wall_border_point calculate_point_with_offset_from_line(const wall_axis_point& start, const wall_axis_point& end, double left_offset)
    {
        double dx = end.x - start.x;
        double dy = end.y - start.y;
        double l = std::sqrt(dx*dx + dy*dy);

        return
        {
            start.x + dy / l * left_offset,
            start.y - dx / l * left_offset
        };
    }

    std::optional<wall_border_point> calculate_lines_intersection(
        const wall_border_point& start1, const wall_border_point& end1,
        const wall_border_point& start2, const wall_border_point& end2
    )
    {
        double A1 = end1.y - start1.y;
        double B1 = start1.x - end1.x;
        double C1 = A1 * start1.x + B1 * start1.y;

        double A2 = end2.y - start2.y;
        double B2 = start2.x - end2.x;
        double C2 = A2 * start2.x + B2 * start2.y;

        double det = A1 * B2 - A2 * B1;
        if (std::abs(det) < 0.0001)
        {
            return std::nullopt;
        }

        double x = (B2 * C1 - B1 * C2) / det;
        double y = (A1 * C2 - A2 * C1) / det;

        return wall_border_point {x, y};
    }

    std::pair<wall_border_point, std::optional<wall_border_point>> calculate_joined_walls_left_border_intersection(
        const wall& w1, const wall& w2,
        const wall_axis_line& l1, const wall_axis_line& l2,
        const wall_axis_point& w1_free_p, const wall_axis_point& common_p, const wall_axis_point& w2_free_p
    )
    {
        double w1_axis_offset = (common_p.index == l1.s? w1.axis_offset : -w1.axis_offset);
        double w2_axis_offset = (common_p.index == l2.s? -w2.axis_offset : w2.axis_offset);

        double w1_offset = w1.width * 0.5 + w1_axis_offset;
        double w2_offset = -(w2.width * 0.5 + w2_axis_offset);

        auto left_w1_offset_p = calculate_point_with_offset_from_line(common_p, w1_free_p, w1_offset);
        auto right_w2_offset_p = calculate_point_with_offset_from_line(common_p, w2_free_p, w2_offset);

        auto intersection_p = calculate_lines_intersection(
            left_w1_offset_p,
            left_w1_offset_p + wall_border_point(w1_free_p - common_p),
            right_w2_offset_p,
            right_w2_offset_p + wall_border_point(w2_free_p - common_p)
        );

        if (intersection_p)
        {
            return { intersection_p.value(), std::nullopt };
        }
        else
        {
           // borders are coincident
            if ((w1_offset + w2_offset) < 0.0001)
            {
                return { left_w1_offset_p, std::nullopt }; // should be same as right_w2_offset_p
            }
            else
            {
                return { left_w1_offset_p, right_w2_offset_p };
            }
        }
    }

    double normailze_angle(double a)
    {
        while (a < -std::numbers::pi) a += 2 * std::numbers::pi;
        while (a > std::numbers::pi) a -= 2 * std::numbers::pi;

        return a;
    }

    double inverse_angle(double a)
    {
        return normailze_angle(a - std::numbers::pi);
    }
}

wall_calculator::wall_calculator(model::floor &floor)
    : _floor { floor }
{
}

void wall_calculator::recalculate_all_walls()
{
    // recalculate wall axis joints
    for (auto&& [_, a] : _floor.data().items<wall_axis_point>())
    {
        _floor.data().annotation(a.index).connected_walls = std::nullopt;
    }

    for (auto&& [_, w] : _floor.data().items<wall>())
    {
        recalculate_wall_joints(w);
    }


    // clean-up ref count for previously generated items
    for (auto& pair : _points_cache)
    {
        pair.second.refcount = 0;
    }

    // recalculate new wall borders
    std::map<wall::index_t, double> walls_directions;
    std::vector<wall_axis_point_locator> processed_pls;

    for (auto&& [_, w] : _floor.data().items<wall>())
    {
        const auto& a = _floor.data().get(w.axis);

        // start joint points
        size_t start_joints_num = _floor.data().annotation(a.s).connected_walls->size() - 1;
        if (start_joints_num == 0)
        {
            calculate_stub_wall_start_borders(w);
        }
        else 
        {
            w.start_stub = {};

            wall_axis_point_locator wall_start_pl { w.index, &wall_axis_line::s };
            if (std::ranges::find(processed_pls, wall_start_pl) == processed_pls.end())
            {
                if (start_joints_num == 1)
                {            
                    calculate_joined_2_walls_borders(wall_start_pl, processed_pls);
                }
                else
                {
                    calculate_joined_n_walls_borders(a.s, walls_directions, processed_pls);
                }
            }
        }

        // end joint points
        size_t end_joints_num = _floor.data().annotation(a.e).connected_walls->size() - 1;
        if (end_joints_num == 0)
        {
            calculate_stub_wall_end_borders(w);
        }
        else
        {
            w.end_stub = {};

            wall_axis_point_locator wall_end_pl { w.index, &wall_axis_line::e };
            if (std::ranges::find(processed_pls, wall_end_pl) == processed_pls.end())
            {
                if (end_joints_num == 1)
                {
                    
                    calculate_joined_2_walls_borders(wall_end_pl, processed_pls);
                }
                else
                {
                    calculate_joined_n_walls_borders(a.e, walls_directions, processed_pls);
                }
            }
        }
    }

    // erase unreacheable items
    for (auto it = _borders_cache.begin(); it != _borders_cache.end(); ) 
    {
        if (it->second.refcount == 0)
        {
            // cached point is not used anymore
            _floor.data().erase(it->second.index);
            it = _borders_cache.erase(it); 
        }
        else
        {
            ++it;
        }
    }

    for (auto it = _points_cache.begin(); it != _points_cache.end(); ) 
    {
        if (it->second.refcount == 0)
        {
            // cached point is not used anymore
            _floor.data().erase(it->second.index);
            it = _points_cache.erase(it); 
        }
        else
        {
            ++it;
        }
    }
}

void wall_calculator::calculate_stub_wall_start_borders(wall& w)
{
    const auto& axis = _floor.data().get(w.axis);

    const auto& start_p = _floor.data().get(axis.s);
    const auto& end_p = _floor.data().get(axis.e);

    const double left_offset = w.width * 0.5 + w.axis_offset;
    const double right_offset = -(w.width * 0.5 - w.axis_offset);

    const auto left_p = calculate_point_with_offset_from_line(start_p, end_p, left_offset);
    const auto right_p = calculate_point_with_offset_from_line(start_p, end_p, right_offset);

    assign_point_and_borders_to_walls(
        w, &wall::left, &wall_border_line::s,
        w, &wall::start_stub, &wall_border_line::s,
        left_p
    );
    assign_point_and_borders_to_walls(
        w, &wall::right, &wall_border_line::s,
        w, &wall::start_stub, &wall_border_line::e, 
        right_p
    );
}

void wall_calculator::calculate_stub_wall_end_borders(wall& w)
{
    const auto& axis = _floor.data().get(w.axis);

    const auto& start_p = _floor.data().get(axis.s);
    const auto& end_p = _floor.data().get(axis.e);

    double left_offset = -(w.width * 0.5 + w.axis_offset);
    double right_offset = w.width * 0.5 - w.axis_offset;

    auto left_p = calculate_point_with_offset_from_line(end_p, start_p, left_offset);
    auto right_p = calculate_point_with_offset_from_line(end_p, start_p, right_offset);

    assign_point_and_borders_to_walls(
        w, &wall::left, &wall_border_line::e,
        w, &wall::end_stub, &wall_border_line::s,
        left_p
    );
    assign_point_and_borders_to_walls(
        w, &wall::right, &wall_border_line::e,
        w, &wall::end_stub, &wall_border_line::e, 
        right_p
    );
}

void wall_calculator::recalculate_wall_joints(wall& w)
{
    const auto& axis = _floor.data().get(w.axis);

    auto recalculate_joints_for_point = [&](wall_axis_point::index_t pid) {
        auto& sud = _floor.data().annotation(pid);
        if (!sud.connected_walls)
        {
            std::vector<wall_axis_point_locator> pls;

            for (auto&& [_, w2] : _floor.data().items<wall>())
            {
                const auto& a2 = _floor.data().get(w2.axis);            
                if (pid == a2.s)
                {
                    pls.push_back(wall_axis_point_locator { w2.index, &wall_axis_line::s });
                }
                else if (pid == a2.e)
                {
                    pls.push_back(wall_axis_point_locator { w2.index, &wall_axis_line::e });
                }
            }
            
            for (const auto& pl : pls)
            {
                const auto& w = _floor.data().get(pl.wall_id);
                const auto& a = _floor.data().get(w.axis);
                auto apid = a.*(pl.point_on_axis_ptr);
                _floor.data().annotation(apid).connected_walls = pls;
            }
        }
    };

    recalculate_joints_for_point(axis.s);
    recalculate_joints_for_point(axis.e);
}

void wall_calculator::calculate_joined_2_walls_borders(
    const wall_axis_point_locator& apl,
    std::vector<wall_axis_point_locator>& processed_apls
)
{
    auto jw = get_joined_walls_points(apl);

    const auto& wall1_free_p = _floor.data().get(jw.wall1_free_p);
    const auto& walls_common_p = _floor.data().get(jw.walls_common_p);
    const auto& wall2_free_p = _floor.data().get(jw.wall2_free_p);

    auto& wall1 = _floor.data().get(apl.wall_id);
    auto& wall2 = _floor.data().get(jw.wall2_apl.wall_id);

    const auto& axis1 = _floor.data().get(wall1.axis);
    const auto& axis2 = _floor.data().get(wall2.axis);

    const auto left_intersection_p = calculate_joined_walls_left_border_intersection(
        wall1, wall2,
        axis1, axis2,
        wall1_free_p, walls_common_p, wall2_free_p
    );

    const auto right_intersection_p = calculate_joined_walls_left_border_intersection(
        wall2, wall1,
        axis2, axis1,
        wall2_free_p, walls_common_p, wall1_free_p
    );

    assign_left_intersection_point(wall1, apl.point_on_axis_ptr, wall2, jw.wall2_apl.point_on_axis_ptr, left_intersection_p);
    assign_left_intersection_point(wall2, jw.wall2_apl.point_on_axis_ptr, wall1, apl.point_on_axis_ptr, right_intersection_p);
    
    processed_apls.push_back(apl);
    processed_apls.push_back(jw.wall2_apl);
}

void wall_calculator::calculate_joined_n_walls_borders(
    wall_axis_point::index_t apid,
    std::map<model::shape::wall::index_t, double>& walls_directions,
    std::vector<wall_axis_point_locator>& processed_apls
)
{
    const auto& ud = _floor.data().annotation(apid);

    struct wf_direction
    {
        wall_axis_point_locator apl;
        double direction;
    };
    std::vector<wf_direction> w_joints_directions;

    for (const auto& apl : ud.connected_walls.value())
    {
        double a = 0;
        if (const auto it = walls_directions.find(apl.wall_id); it != walls_directions.end())
        {
            a = it->second;
        }
        else
        {
            const auto& w = _floor.data().get(apl.wall_id);
            a = calculate_line_direction(_floor.data().get(w.axis), _floor);
            walls_directions[apl.wall_id] = a;
        }

        if (apl.point_on_axis_ptr == &wall_axis_line::e)
        {
            a = inverse_angle(a);
        }

        w_joints_directions.push_back({ apl, a });
    }

    std::ranges::sort(
        w_joints_directions,
        [](const wf_direction& lhs, const wf_direction& rhs) { return lhs.direction < rhs.direction; }
    );

    for (int i = 0; i < w_joints_directions.size(); ++i)
    {
        int prev_i = i - 1;
        if (prev_i < 0)
        {
            prev_i = w_joints_directions.size() - 1;
        }

        const auto& apl1 = w_joints_directions[i].apl;
        const auto& apl2 = w_joints_directions[prev_i].apl;

        auto& w1 = _floor.data().get(apl1.wall_id);
        auto& w2 = _floor.data().get(apl2.wall_id);

        const auto& l1 = _floor.data().get(w1.axis);
        const auto& l2 = _floor.data().get(w2.axis);

        const auto& wall1_free_p = _floor.data().get(apl1.point_on_axis_ptr == &wall_axis_line::s ? l1.e : l1.s);
        const auto& walls_common_p = _floor.data().get(apl1.point_on_axis_ptr == &wall_axis_line::s ? l1.s : l1.e);
        const auto& wall2_free_p = _floor.data().get(apl2.point_on_axis_ptr == &wall_axis_line::s ? l2.e : l2.s);

        const auto left_intersection_p = calculate_joined_walls_left_border_intersection(
            w1, w2,
            l1, l2,
            wall1_free_p, walls_common_p, wall2_free_p
        );

        assign_left_intersection_point(w1, apl1.point_on_axis_ptr, w2, apl2.point_on_axis_ptr, left_intersection_p);
    }

    std::ranges::copy(ud.connected_walls.value(), std::back_inserter(processed_apls));
}

wall_calculator::joined_walls wall_calculator::get_joined_walls_points(const wall_axis_point_locator& apl)
{
    joined_walls result;

    const auto& wall1 = _floor.data().get(apl.wall_id);
    const auto& axis1 = _floor.data().get(wall1.axis);

    result.wall1_free_p = axis1.*(opposite(apl.point_on_axis_ptr));
    result.walls_common_p = axis1.*(apl.point_on_axis_ptr);

    const auto& common_p_data = _floor.data().annotation(result.walls_common_p);
    for (const auto& w2apl : common_p_data.connected_walls.value())
    {
        // assuming wall1 has exactly one joined wall. i.e. w1_joints.size() == 2
        if (w2apl.wall_id != apl.wall_id)
        {
            const auto& wall2 = _floor.data().get(w2apl.wall_id);
            const auto& axis2 = _floor.data().get(wall2.axis);

            result.wall2_free_p = axis2.*(opposite(w2apl.point_on_axis_ptr));

            result.wall2_apl = w2apl;

            break;
        }
    }

    return result;
}

wall_border_point::index_t wall_calculator::find_or_create_point(
    const border_point_geometry_id& id,
    const wall_border_point& p
)
{
    auto [it, inserted] = _points_cache.try_emplace(id);

    if (inserted)
    {
        it->second.index = _floor.data().put(p);
        it->second.refcount = 1;
    }
    else
    {
        auto& existing = _floor.data().get(it->second.index);
        existing.x = p.x;
        existing.y = p.y;

        it->second.refcount++;
    }

    return it->second.index;
}

void wall_calculator::assign_point_and_borders_to_walls(
    wall& w1, wall_border_line_ptr b1_ptr, point_on_wall_border_ptr p1_ptr,
    wall& w2, wall_border_line_ptr b2_ptr, point_on_wall_border_ptr p2_ptr,
    const wall_border_point &point
)
{
    auto& b1 = find_or_create_border({ w1.index, b1_ptr });
    auto& b2 = find_or_create_border({ w2.index, b2_ptr });

    w1.*b1_ptr = b1.index;
    w2.*b2_ptr = b2.index;

    const auto pi = find_or_create_point({b1.index, p1_ptr, b2.index, p2_ptr}, point);
    b1.*p1_ptr = pi;
    b2.*p2_ptr = pi;

    auto& bpud = _floor.data().annotation(pi);

    bpud.point_locators[0].wall_id = w1.index;
    bpud.point_locators[0].border_ptr = b1_ptr;
    bpud.point_locators[0].point_on_border_ptr = p1_ptr;

    bpud.point_locators[1].wall_id = w2.index;
    bpud.point_locators[1].border_ptr = b2_ptr;
    bpud.point_locators[1].point_on_border_ptr = p2_ptr;
}

wall_border_line &wall_calculator::find_or_create_border(const wall_border_geometry_id &id)
{
    auto [it, inserted] = _borders_cache.try_emplace(id);

    if (inserted)
    {
        it->second.index = _floor.data().make<wall_border_line>(wall_border_point::index_t {}, wall_border_point::index_t {});
        it->second.refcount = 1;
    }
    else
    {
        it->second.refcount++;
    }
    return _floor.data().get(it->second.index);
}

void wall_calculator::assign_left_intersection_point(
    wall &wall1, point_on_wall_axis_ptr wall1_location,
    wall &wall2, point_on_wall_axis_ptr wall2_location,
    const std::pair<wall_border_point, std::optional<wall_border_point>> &intersection_pair)
{
    const auto border1_ptr = wall1_location == &wall_axis_line::s ? &wall::left : &wall::right;
    const auto border2_ptr = wall2_location == &wall_axis_line::s ? &wall::right : &wall::left;

    const auto border1_point_ptr = wall1_location == &wall_axis_line::s ? &wall_border_line::s : &wall_border_line::e;
    const auto border2_point_ptr = wall2_location == &wall_axis_line::s ? &wall_border_line::s : &wall_border_line::e;

    assign_walls_intersection_pair(
        wall1, border1_ptr, border1_point_ptr,
        wall2, border2_ptr, border2_point_ptr,
        intersection_pair
    );
}

void wall_calculator::assign_walls_intersection_pair(
    wall& wall1, wall_border_line_ptr wall1_line_ptr, point_on_wall_border_ptr wall1_point_ptr,
    wall& wall2, wall_border_line_ptr wall2_line_ptr, point_on_wall_border_ptr wall2_point_ptr,
    const std::pair<wall_border_point, std::optional<wall_border_point>> &intersection_pair
)
{
    if (!intersection_pair.second)
    {
        assign_point_and_borders_to_walls(
            wall1, wall1_line_ptr, wall1_point_ptr,
            wall2, wall2_line_ptr, wall2_point_ptr,
            intersection_pair.first
        );
    }
    else
    {
        const auto w1_stub_border_ptr = (wall1_point_ptr == &wall_border_line::s) ?
            &wall::start_stub : &wall::end_stub;

        assign_point_and_borders_to_walls(
            wall1, wall1_line_ptr, wall1_point_ptr,
            wall1, w1_stub_border_ptr, &wall_border_line::s,
            intersection_pair.first
        );
        assign_point_and_borders_to_walls(
            wall2, wall2_line_ptr, wall2_point_ptr,
            wall1, w1_stub_border_ptr, &wall_border_line::e,
            intersection_pair.second.value()
        );
    }
}

wall_calculator::border_point_geometry_id::border_point_geometry_id(
    wall_border_line::index_t b1_id, point_on_wall_border_ptr b_p1_ptr,
    wall_border_line::index_t b2_id, point_on_wall_border_ptr b_p2_ptr
)
    : border1_id { b1_id < b2_id ? b1_id : b2_id }
    , border_point1_ptr { b1_id < b2_id ? b_p1_ptr : b_p2_ptr }
    , border2_id { b1_id < b2_id ? b2_id : b1_id }
    , border_point2_ptr { b1_id < b2_id ? b_p2_ptr : b_p1_ptr }
{
}

wall_calculator::wall_border_geometry_id::wall_border_geometry_id(wall::index_t w_id, wall_border_line_ptr w_b_ptr)
    : wall_id { w_id }
    , wall_border_ptr { w_b_ptr }
{
}

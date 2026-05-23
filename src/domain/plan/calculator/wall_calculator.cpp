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
    // clean-up ref count for previously generated items
    for (auto& pair : _points_cache)
    {
        pair.second.refcount = 0;
    }

    for (auto& pair : _borders_cache)
    {
        pair.second.refcount = 0;
    }

    // recalculate new wall borders
    walls_joints joints;
    std::map<wall_axis_line::index_t, double> wall_axis_directions;
    std::map<wall_axis_line::index_t, wall::index_t> wall_axis_owners;

    for (auto& w : _floor.data().items<wall>())
    {
        recalculate_wall_joints(w.second, joints);
        wall_axis_owners[w.second.axis] = w.second.index;
    }

    std::vector<wall_finish_id> processed_fids;
    for (auto& w : _floor.data().items<wall>())
    {
        // start joint points
        wall_finish_id wall_start_fid { w.second.axis, wall_location::start };
        size_t start_joints_num = joints[wall_start_fid].size() - 1;
        if (start_joints_num == 0)
        {
            calculate_stub_wall_start_borders(w.second);
        }
        else 
        {
            w.second.start_stub = {};

            if (std::ranges::find(processed_fids, wall_start_fid) == processed_fids.end())
            {
                if (start_joints_num == 1)
                {            
                    calculate_joined_2_walls_borders(wall_start_fid, joints, wall_axis_owners, processed_fids);
                }
                else
                {
                    calculate_joined_n_walls_borders(wall_start_fid, joints, wall_axis_owners, wall_axis_directions, processed_fids);
                }
            }
        }

        // end joint points
        wall_finish_id wall_end_fid { w.second.axis, wall_location::end };
        size_t end_joints_num = joints[wall_end_fid].size() - 1;
        if (end_joints_num == 0)
        {
            calculate_stub_wall_end_borders(w.second);
        }
        else
        {
            w.second.end_stub = {};

            if (std::ranges::find(processed_fids, wall_end_fid) == processed_fids.end())
            {
                if (end_joints_num == 1)
                {
                    
                    calculate_joined_2_walls_borders(wall_end_fid, joints, wall_axis_owners, processed_fids);
                }
                else
                {
                    calculate_joined_n_walls_borders(wall_end_fid, joints, wall_axis_owners, wall_axis_directions, processed_fids);
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

    auto& stub_border = find_or_create_border({ w.index, &wall::start_stub });
    auto& left_border = find_or_create_border({ w.index, &wall::left });
    auto& right_border = find_or_create_border({ w.index, &wall::right });

    assign_point_to_borders(left_border, &wall_border_line::s, stub_border, &wall_border_line::s, left_p);
    assign_point_to_borders(right_border, &wall_border_line::s, stub_border, &wall_border_line::e, right_p);

    w.left = left_border.index;
    w.start_stub = stub_border.index;
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

    auto& stub_border = find_or_create_border({ w.index, &wall::end_stub });
    auto& left_border = find_or_create_border({ w.index, &wall::left });
    auto& right_border = find_or_create_border({ w.index, &wall::right });

    assign_point_to_borders(left_border, &wall_border_line::e, stub_border, &wall_border_line::s, left_p);
    assign_point_to_borders(right_border, &wall_border_line::e, stub_border, &wall_border_line::e, right_p);

    w.right = right_border.index;
    w.end_stub = stub_border.index;
}

void wall_calculator::recalculate_wall_joints(wall& w, walls_joints& joints)
{
    const auto axis = _floor.data().get(w.axis);

    wall_finish_id current_start_id { w.axis, wall_location::start };
    if (joints.find(current_start_id) == joints.end())
    {
        std::vector<wall_finish_id> start_connected_walls;

        for (auto& a2 : _floor.data().items<wall_axis_line>())
        {
            if (axis.s == a2.second.s)
            {
                start_connected_walls.push_back(wall_finish_id { a2.second.index, wall_location::start });
            }
            else if (axis.s == a2.second.e)
            {
                start_connected_walls.push_back(wall_finish_id { a2.second.index, wall_location::end });
            }
        }
        
        for (const auto& cw : start_connected_walls)
        {
            joints[cw] = start_connected_walls;
        }
    }


    wall_finish_id current_end_id { w.axis, wall_location::end };
    if (joints.find(current_end_id) == joints.end())
    {
        std::vector<wall_finish_id> end_connected_walls;

        for (auto& a2 : _floor.data().items<wall_axis_line>())
        {
            if (axis.e == a2.second.s)
            {
                end_connected_walls.push_back(wall_finish_id { a2.second.index, wall_location::start });
            }
            else if (axis.e == a2.second.e)
            {
                end_connected_walls.push_back(wall_finish_id { a2.second.index, wall_location::end });
            }
        }
        
        for (const auto& cw : end_connected_walls)
        {
            joints[cw] = end_connected_walls;
        }
    }
}

void wall_calculator::calculate_joined_2_walls_borders(
    wall_finish_id fid,
    const walls_joints& joints,
    const std::map<wall_axis_line::index_t, wall::index_t>& wall_axis_owners,
    std::vector<wall_finish_id>& processed_fids
)
{
    auto jw = get_joined_walls_points(fid, joints);

    const auto& wall1_free_p = _floor.data().get(jw.wall1_free_p);
    const auto& walls_common_p = _floor.data().get(jw.walls_common_p);
    const auto& wall2_free_p = _floor.data().get(jw.wall2_free_p);

    const auto wall1_id = wall_axis_owners.at(fid.axis_id);
    const auto wall2_id = wall_axis_owners.at(jw.wall2_fid.axis_id);

    auto& wall1 = _floor.data().get(wall1_id);
    auto& wall2 = _floor.data().get(wall2_id);

    const auto& axis1 = _floor.data().get(fid.axis_id);
    const auto& axis2 = _floor.data().get(jw.wall2_fid.axis_id);

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

    assign_left_intersection_point(wall1, fid.location, wall2, jw.wall2_fid.location, left_intersection_p);
    assign_left_intersection_point(wall2, jw.wall2_fid.location, wall1, fid.location, right_intersection_p);
    
    processed_fids.push_back(fid);
    processed_fids.push_back(jw.wall2_fid);
}

void wall_calculator::calculate_joined_n_walls_borders(
    wall_finish_id fid,
    const walls_joints& joints,
    const std::map<wall_axis_line::index_t, wall::index_t>& wall_axis_owners,
    std::map<wall_axis_line::index_t, double>& wall_axis_directions,
    std::vector<wall_finish_id>& processed_fids
)
{
    const auto& w_joints = joints.at(fid);

    struct wf_direction
    {
        wall_finish_id wfid;
        double direction;
    };
    std::vector<wf_direction> w_joints_directions;

    for (const auto& wfid : w_joints)
    {
        double a = 0;
        if (const auto it = wall_axis_directions.find(wfid.axis_id); it != wall_axis_directions.end())
        {
            a = it->second;
        }
        else
        {
            a = calculate_line_direction(_floor.data().get(wfid.axis_id), _floor);
            wall_axis_directions[wfid.axis_id] = a;
        }

        if (wfid.location == wall_location::end)
        {
            a = inverse_angle(a);
        }

        w_joints_directions.push_back({ wfid, a });
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

        const auto& wfid1 = w_joints_directions[i].wfid;
        const auto& wfid2 = w_joints_directions[prev_i].wfid;

        const auto w1_id = wall_axis_owners.at(wfid1.axis_id);
        const auto w2_id = wall_axis_owners.at(wfid2.axis_id);

        auto& w1 = _floor.data().get(w1_id);
        auto& w2 = _floor.data().get(w2_id);

        const auto& l1 = _floor.data().get(wfid1.axis_id);
        const auto& l2 = _floor.data().get(wfid2.axis_id);

        const auto& wall1_free_p = _floor.data().get(wfid1.location == wall_location::start ? l1.e : l1.s);
        const auto& walls_common_p = _floor.data().get(wfid1.location == wall_location::start ? l1.s : l1.e);
        const auto& wall2_free_p = _floor.data().get(wfid2.location == wall_location::start ? l2.e : l2.s);

        const auto left_intersection_p = calculate_joined_walls_left_border_intersection(
            w1, w2,
            l1, l2,
            wall1_free_p, walls_common_p, wall2_free_p
        );

        assign_left_intersection_point(w1, wfid1.location, w2, wfid2.location, left_intersection_p);
    }

    std::ranges::copy(w_joints, std::back_inserter(processed_fids));
}

wall_calculator::joined_walls wall_calculator::get_joined_walls_points(const wall_finish_id& wfid, const walls_joints& joints)
{
    joined_walls result;

    const auto& axis1 = _floor.data().get(wfid.axis_id);
    if (wfid.location == wall_location::start)
    {
        result.wall1_free_p = axis1.e;
        result.walls_common_p = axis1.s;
    }
    else
    {
        result.wall1_free_p = axis1.s;
        result.walls_common_p = axis1.e;
    }

    const auto& w1_joints = joints.at(wfid);
    for (const auto& w2fid : w1_joints)
    {
        // assuming wall1 has exactly one joined wall. i.e. w1_joints.size() == 2
        if (w2fid.axis_id != wfid.axis_id)
        {
            const auto& axis2 = _floor.data().get(w2fid.axis_id);

            if (w2fid.location == wall_location::start)
            {
                result.wall2_free_p = axis2.e;
            }
            else
            {
                result.wall2_free_p = axis2.s;
            }

            result.wall2_fid = w2fid;

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

void wall_calculator::assign_point_to_borders(
    wall_border_line &b1, point_on_border_ptr p1_ptr,
    wall_border_line &b2, point_on_border_ptr p2_ptr,
    const wall_border_point &point
)
{
    const auto pi = find_or_create_point({b1.index, p1_ptr, b2.index, p2_ptr}, point);
    b1.*p1_ptr = pi;
    b2.*p2_ptr = pi;
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
    wall &wall1, wall_location wall1_location,
    wall &wall2, wall_location wall2_location,
    const std::pair<wall_border_point, std::optional<wall_border_point>> &intersection_pair)
{
    if (wall1_location == wall_location::start && wall2_location == wall_location::start)
    {
        assign_walls_intersection_pair(
            wall1, &wall::left, &wall_border_line::s,
            wall2, &wall::right, &wall_border_line::s,
            intersection_pair
        );
    }
    else if (wall1_location == wall_location::start && wall2_location == wall_location::end)
    {
        assign_walls_intersection_pair(
            wall1, &wall::left, &wall_border_line::s,
            wall2, &wall::left, &wall_border_line::e,
            intersection_pair
        );
    }
    else if (wall1_location == wall_location::end && wall2_location == wall_location::start)
    {
        assign_walls_intersection_pair(
            wall1, &wall::right, &wall_border_line::e, 
            wall2, &wall::right, &wall_border_line::s, 
            intersection_pair
        );
    }
    else /*if (wall1_location == wall_location::end && wall2_location == wall_location::end) */
    {
        assign_walls_intersection_pair(
            wall1, &wall::right, &wall_border_line::e,
            wall2, &wall::left, &wall_border_line::e,
            intersection_pair
        );
    }
}

void wall_calculator::assign_walls_intersection_pair(
    model::wall& wall1, wall_border_line_ptr wall1_line_ptr, point_on_border_ptr wall1_point_ptr,
    model::wall& wall2, wall_border_line_ptr wall2_line_ptr, point_on_border_ptr wall2_point_ptr,
    const std::pair<wall_border_point, std::optional<wall_border_point>> &intersection_pair
)
{
    if (!intersection_pair.second)
    {
        auto& w1_border = find_or_create_border({ wall1.index, wall1_line_ptr });
        auto& w2_border = find_or_create_border({ wall2.index, wall2_line_ptr });

        assign_point_to_borders(w1_border, wall1_point_ptr, w2_border, wall2_point_ptr, intersection_pair.first);
 
        wall1.*wall1_line_ptr = w1_border.index;
        wall2.*wall2_line_ptr = w2_border.index;
    }
    else
    {
        const auto w1_stub_border_ptr = (wall1_point_ptr == &wall_border_line::s) ?
            &wall::start_stub : &wall::end_stub;

        auto& stub_border = find_or_create_border({ wall1.index, w1_stub_border_ptr });
        auto& w1_border = find_or_create_border({ wall1.index, wall1_line_ptr });
        auto& w2_border = find_or_create_border({ wall2.index, wall2_line_ptr });

        assign_point_to_borders(w1_border, wall1_point_ptr, stub_border, &wall_border_line::s, intersection_pair.first);
        assign_point_to_borders(w2_border, wall2_point_ptr, stub_border, &wall_border_line::e, intersection_pair.second.value());

        wall1.*wall1_line_ptr = w1_border.index;
        wall1.*w1_stub_border_ptr = stub_border.index;
    }
}

wall_calculator::border_point_geometry_id::border_point_geometry_id(
    wall_border_line::index_t b1_id, point_on_border_ptr b_p1_ptr,
    wall_border_line::index_t b2_id, point_on_border_ptr b_p2_ptr
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

std::ostream &model::operator<<(std::ostream &os, corecad::model::property<model::wall_border_point::index_t, model::wall_border_line> model::wall_border_line::*pow)
{
    if (pow == nullptr) os << 'N';
    else if (pow == &model::wall_border_line::s) os << "S";
    else if (pow == &model::wall_border_line::e) os << "E";
    else os << "??";

    return os;
}

std::ostream &model::operator<<(std::ostream &os, model::wall_border_line::index_t model::wall::*pow)
{
    if (pow == nullptr) os << 'N';
    else if (pow == &model::wall::left) os << "L";
    else if (pow == &model::wall::right) os << "R";
    else if (pow == &model::wall::start_stub) os << "SS";
    else if (pow == &model::wall::end_stub) os << "ES";
    else os << "??";

    return os;
}

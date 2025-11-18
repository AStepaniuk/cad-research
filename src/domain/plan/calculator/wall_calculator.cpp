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
    enum class wall_location { start, end };

    struct wall_finish_id
    {
        wall::index_t wall_index;
        wall_location location;
    };

    inline bool operator<(const wall_finish_id& lhs, const wall_finish_id& rhs)
    {
        return std::tie(lhs.wall_index, lhs.location) < std::tie(rhs.wall_index, rhs.location);
    }

    inline bool operator==(const wall_finish_id& lhs, const wall_finish_id& rhs)
    {
        return std::tie(lhs.wall_index, lhs.location) == std::tie(rhs.wall_index, rhs.location);
    }

    inline bool operator!=(const wall_finish_id& lhs, const wall_finish_id& rhs)
    {
        return !(lhs == rhs);
    }

    using walls_joints = std::map<wall_finish_id, std::vector<wall_finish_id>>;
    void recalculate_wall_joints(wall& w, model::floor& f, walls_joints& joints)
    {

        wall_finish_id current_start_id { w.index, wall_location::start };
        if (joints.find(current_start_id) == joints.end())
        {
            std::vector<wall_finish_id> start_connected_walls;

            for (auto& w2 : f.walls())
            {
                if (w.start == w2.second.start)
                {
                    start_connected_walls.push_back(wall_finish_id { w2.second.index, wall_location::start });
                }
                else if (w.start == w2.second.end)
                {
                    start_connected_walls.push_back(wall_finish_id { w2.second.index, wall_location::end });
                }
            }
            
            for (const auto& cw : start_connected_walls)
            {
                if (cw.location == wall_location::start)
                {
                    f.walls().get(cw.wall_index).start_joints = start_connected_walls.size() - 1;
                }
                else
                {
                    f.walls().get(cw.wall_index).end_joints = start_connected_walls.size() - 1;
                }

                joints[cw] = start_connected_walls;
            }
            std::cout << std::endl;
        }


        wall_finish_id current_end_id { w.index, wall_location::end };
        if (joints.find(current_end_id) == joints.end())
        {
            std::vector<wall_finish_id> end_connected_walls;

            for (auto& w2 : f.walls())
            {
                if (w.end == w2.second.start)
                {
                    end_connected_walls.push_back(wall_finish_id { w2.second.index, wall_location::start });
                }
                else if (w.end == w2.second.end)
                {
                    end_connected_walls.push_back(wall_finish_id { w2.second.index, wall_location::end });
                }
            }
            
            for (const auto& cw : end_connected_walls)
            {
                if (cw.location == wall_location::start)
                {
                    f.walls().get(cw.wall_index).start_joints = end_connected_walls.size() - 1;
                }
                else
                {
                    f.walls().get(cw.wall_index).end_joints = end_connected_walls.size() - 1;
                }

                joints[cw] = end_connected_walls;
            }
            std::cout << std::endl;
        }
    }

    double calculate_wall_direction(const wall& w, const model::floor& f)
    {
        const auto& ps = f.points().get(w.start);
        const auto& pe = f.points().get(w.end);

        return std::atan2f(pe.y - ps.y, pe.x - ps.x);
    }

    vector2d calculate_point_with_offset_from_line(const vector2d& start, const vector2d& end, double left_offset)
    {
        double dx = end.x - start.x;
        double dy = end.y - start.y;
        double l = std::sqrtf(dx*dx + dy*dy);

        return
        {
            start.x + dy / l * left_offset,
            start.y - dx / l * left_offset
        };
    }

    std::optional<vector2d> calculate_lines_intersection(
        const vector2d& start1, const vector2d& end1,
        const vector2d& start2, const vector2d& end2
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

        return vector2d {x, y};
    }

    struct joined_walls
    {
        vector2d::index_t wall1_free_p;
        vector2d::index_t walls_common_p;
        vector2d::index_t wall2_free_p;

        wall_finish_id wall2_fid;
    };
    joined_walls get_joined_walls_points(const wall_finish_id& wfid, const model::floor& floor, const walls_joints& joints)
    {
        joined_walls result;

        const wall& wall1 = floor.walls().get(wfid.wall_index);
        if (wfid.location == wall_location::start)
        {
            result.wall1_free_p = wall1.end;
            result.walls_common_p = wall1.start;
        }
        else
        {
            result.wall1_free_p = wall1.start;
            result.walls_common_p = wall1.end;
        }

        const auto& w1_joints = joints.at(wfid);
        for (const auto& w2fid : w1_joints)
        {
            // assuming wall1 has exactly one joined wall. i.e. w1_joints.size() == 2
            if (w2fid.wall_index != wfid.wall_index)
            {
                const wall& wall2 = floor.walls().get(w2fid.wall_index);

                if (w2fid.location == wall_location::start)
                {
                    result.wall2_free_p = wall2.end;
                }
                else
                {
                    result.wall2_free_p = wall2.start;
                }

                result.wall2_fid = w2fid;

                break;
            }
        }

        return result;
    }

    void calculate_stub_wall_start_borders(wall& w, model::floor &floor)
    {
        const auto& start_p = floor.points().get(w.start);
        const auto& end_p = floor.points().get(w.end);

        double left_offset = w.width * 0.5;
        double right_offset = -w.width * 0.5;

        auto left_p = calculate_point_with_offset_from_line(start_p, end_p, left_offset);
        auto right_p = calculate_point_with_offset_from_line(start_p, end_p, right_offset);

        w.start_left = floor.points().put(left_p);
        w.start_right = floor.points().put(right_p);
    }

    void calculate_stub_wall_end_borders(wall& w, model::floor &floor)
    {
        const auto& start_p = floor.points().get(w.start);
        const auto& end_p = floor.points().get(w.end);

        double left_offset = -w.width * 0.5;
        double right_offset = w.width * 0.5;

        auto left_p = calculate_point_with_offset_from_line(end_p, start_p, left_offset);
        auto right_p = calculate_point_with_offset_from_line(end_p, start_p, right_offset);

        w.end_left = floor.points().put(left_p);
        w.end_right = floor.points().put(right_p);
    }

    vector2d calculate_joined_walls_left_border_intersection(
        const wall& w1, const wall& w2,
        const vector2d& w1_free_p, const vector2d& common_p, const vector2d& w2_free_p
    )
    {
        double w1_offset = w1.width * 0.5;
        double w2_offset = -w2.width * 0.5;

        auto left_w1_offset_p = calculate_point_with_offset_from_line(common_p, w1_free_p, w1_offset);
        auto right_w2_offset_p = calculate_point_with_offset_from_line(common_p, w2_free_p, w2_offset);

        auto intersection_p = calculate_lines_intersection(
            left_w1_offset_p,
            left_w1_offset_p + (w1_free_p - common_p),
            right_w2_offset_p,
            right_w2_offset_p + (w2_free_p - common_p)
        );

        if (intersection_p)
        {
            return intersection_p.value();
        }
        else
        {
            // walls are coincident
            if (w1.width == w2.width)
            {
                return left_w1_offset_p; // should be same as right_w2_offset_p
            }
            else
            {
                throw std::runtime_error("Straight joints for walls with different width are not yet supported");
            }
        }
    }

    void calculate_joined_2_walls_borders(
        wall_finish_id fid,
        model::floor &floor,
        const walls_joints& joints,
        std::vector<wall_finish_id>& processed_fids
    )
    {
        auto jw = get_joined_walls_points(fid, floor, joints);

        const auto& wall1_free_p = floor.points().get(jw.wall1_free_p);
        const auto& walls_common_p = floor.points().get(jw.walls_common_p);
        const auto& wall2_free_p = floor.points().get(jw.wall2_free_p);

        auto& wall1 = floor.walls().get(fid.wall_index);
        auto& wall2 = floor.walls().get(jw.wall2_fid.wall_index);

        const auto left_intersection_p = calculate_joined_walls_left_border_intersection(
            wall1, wall2,
            wall1_free_p, walls_common_p, wall2_free_p
        );

        const auto right_intersection_p = calculate_joined_walls_left_border_intersection(
            wall2, wall1,
            wall2_free_p, walls_common_p, wall1_free_p
        );

        auto left_pid = floor.points().put(left_intersection_p);
        auto right_pid = floor.points().put(right_intersection_p);

        if (fid.location == wall_location::start)
        {
            wall1.start_left = left_pid;
            wall1.start_right = right_pid;
        }
        else
        {
            wall1.end_left = right_pid;
            wall1.end_right = left_pid;
        }

        if (jw.wall2_fid.location == wall_location::start)
        {
            wall2.start_right = left_pid;
            wall2.start_left = right_pid;
        }
        else
        {
            wall2.end_left = left_pid;
            wall2.end_right = right_pid;
        }
        
        processed_fids.push_back(fid);
        processed_fids.push_back(jw.wall2_fid);
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

    void calculate_joined_n_walls_borders(
        wall_finish_id fid,
        model::floor &floor,
        const walls_joints& joints,
        std::map<wall::index_t, double> wall_directions,
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
            if (const auto it = wall_directions.find(wfid.wall_index); it != wall_directions.end())
            {
                a = it->second;
            }
            else
            {
                a = calculate_wall_direction(floor.walls().get(wfid.wall_index), floor);
                wall_directions[wfid.wall_index] = a;
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

            auto& w1 = floor.walls().get(wfid1.wall_index);
            auto& w2 = floor.walls().get(wfid2.wall_index);

            const auto& wall1_free_p = floor.points().get(wfid1.location == wall_location::start ? w1.end : w1.start);
            const auto& walls_common_p = floor.points().get(wfid1.location == wall_location::start ? w1.start : w1.end);
            const auto& wall2_free_p = floor.points().get(wfid2.location == wall_location::start ? w2.end : w2.start);

            const auto left_intersection_p = calculate_joined_walls_left_border_intersection(
                w1, w2,
                wall1_free_p, walls_common_p, wall2_free_p
            );

            auto left_pid = floor.points().put(left_intersection_p);
            if (wfid1.location == wall_location::start)
            {
                w1.start_left = left_pid;
            }
            else
            {
                w1.end_right = left_pid;
            }

            if (wfid2.location == wall_location::start)
            {
                w2.start_right = left_pid;
            }
            else
            {
                w2.end_left = left_pid;
            }
        }

        std::ranges::copy(w_joints, std::back_inserter(processed_fids));
    }
}


void wall_calculator::recalculate_all_walls(model::floor &floor)
{
    walls_joints joints;
    std::map<wall::index_t, double> wall_directions;

    for (auto& w : floor.walls())
    {
        recalculate_wall_joints(w.second, floor, joints);
    }

    std::vector<wall_finish_id> processed_fids;
    for (auto& w : floor.walls())
    {
        // start joint points
        wall_finish_id wall_start_fid { w.second.index, wall_location::start };
        if (w.second.start_joints == 0)
        {
            calculate_stub_wall_start_borders(w.second, floor);
        }
        else 
        {
            if (std::ranges::find(processed_fids, wall_start_fid) == processed_fids.end())
            {
                if (w.second.start_joints == 1)
                {
                    
                    calculate_joined_2_walls_borders(wall_start_fid, floor, joints, processed_fids);
                }
                else
                {
                    calculate_joined_n_walls_borders(wall_start_fid, floor, joints, wall_directions, processed_fids);
                }
            }
        }

        // end joint points
        wall_finish_id wall_end_fid { w.second.index, wall_location::end };
        if (w.second.end_joints == 0)
        {
            calculate_stub_wall_end_borders(w.second, floor);
        }
        else
        {
            if (std::ranges::find(processed_fids, wall_end_fid) == processed_fids.end())
            {
                if (w.second.end_joints == 1)
                {
                    
                    calculate_joined_2_walls_borders(wall_end_fid, floor, joints, processed_fids);
                }
                else
                {
                    calculate_joined_n_walls_borders(wall_end_fid, floor, joints, wall_directions, processed_fids);
                }
            }
        }
    }
}

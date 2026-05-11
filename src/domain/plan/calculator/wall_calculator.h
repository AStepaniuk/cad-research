#pragma once

#include <unordered_map>
#include <map>
#include <iostream>

#include "floor.h"
#include "member_hasher.h"

namespace domain { namespace plan { namespace calculator
{
    class wall_calculator
    {
        model::floor& _floor;

    public:
        wall_calculator(model::floor& floor);

        void recalculate_all_walls();

    private:
        void calculate_stub_wall_start_borders(model::wall& w);
        void calculate_stub_wall_end_borders(model::wall& w);

        enum class wall_location { start, end };
        struct wall_finish_id
        {
            model::wall::index_t wall_index;
            wall_location location;

            auto operator<=>(const wall_finish_id&) const = default;
        };

        using walls_joints = std::map<wall_finish_id, std::vector<wall_finish_id>>;

        struct joined_walls
        {
            model::wall_axis_point::index_t wall1_free_p;
            model::wall_axis_point::index_t walls_common_p;
            model::wall_axis_point::index_t wall2_free_p;

            wall_finish_id wall2_fid;
        };

        void recalculate_wall_joints(model::wall& w, walls_joints& joints);
        void calculate_joined_2_walls_borders(wall_finish_id fid, const walls_joints& joints, std::vector<wall_finish_id>& processed_fids);
        void calculate_joined_n_walls_borders(
            wall_finish_id fid,
            const walls_joints& joints,
            std::map<model::wall::index_t, double> wall_directions,
            std::vector<wall_finish_id>& processed_fids
        );

        joined_walls get_joined_walls_points(const wall_finish_id& wfid, const walls_joints& joints);

        //enum class point_on_wall_location { none, start_right, start_left, end_right, end_left };
        using point_on_wall_ptr = model::wall_border_point::index_t model::wall::*;
        struct wall_point_geometry_id
        {
            wall_point_geometry_id(model::wall::index_t w_id, point_on_wall_ptr w_p_ptr);
            wall_point_geometry_id(model::wall::index_t w1_id, point_on_wall_ptr w1_p_ptr, model::wall::index_t w2_id, point_on_wall_ptr w2_p_ptr);

            auto operator<=>(const wall_point_geometry_id&) const = default;

            model::wall::index_t wall1_id;
            point_on_wall_ptr wall1_point_ptr;

            model::wall::index_t wall2_id;
            point_on_wall_ptr wall2_point_ptr;
        };

        struct wall_point_geometry_id_hasher
        {
            size_t operator()(const domain::plan::calculator::wall_calculator::wall_point_geometry_id& obj) const noexcept
            {
                size_t h1 = std::hash<model::wall::index_t>{}(obj.wall1_id);
                size_t h2 = corecad::util::member_hasher{}(obj.wall1_point_ptr);
                size_t h3 = std::hash<model::wall::index_t>{}(obj.wall2_id);
                size_t h4 = corecad::util::member_hasher{}(obj.wall2_point_ptr);

                auto hash_combine = [](size_t& seed, size_t v) {
                    seed ^= v + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

                size_t seed = h1;
                hash_combine(seed, h2);
                hash_combine(seed, h3);
                hash_combine(seed, h4);
                return seed;
            }
        };

        friend std::ostream& operator<<(std::ostream& os, point_on_wall_ptr pow)
        {
            if (pow == nullptr) os << 'N';
            else if (pow == &model::wall::start_right) os << "SR";
            else if (pow == &model::wall::start_left) os << "SL";
            else if (pow == &model::wall::end_right) os << "ER";
            else if (pow == &model::wall::end_left) os << "EL";
            else os << "??";

            return os;
        }

        friend std::ostream& operator<<(std::ostream& os, const wall_point_geometry_id& id)
        {
            return os << "wid1:" << id.wall1_id << " wl1:" << id.wall1_point_ptr << " wid2:" << id.wall2_id << " wl2:" << id.wall1_point_ptr;
        }

        struct wall_point_info
        {
            model::wall_border_point::index_t index;
            size_t refcount;
        };

        std::unordered_map<wall_point_geometry_id, wall_point_info, wall_point_geometry_id_hasher> _points_cache;

        model::wall_border_point::index_t find_or_create_point(
            const wall_point_geometry_id& id,
            const model::wall_border_point& p
        );

        void assign_left_intersection_point(
            model::wall& wall1, wall_location wall1_location,
            model::wall& wall2, wall_location wall2_location,
            const std::pair<model::wall_border_point, std::optional<model::wall_border_point>>& intersection_pair
        );

        void assign_walls_intersection_pair(
            model::wall& wall1, point_on_wall_ptr wall1_point_ptr,
            model::wall& wall2, point_on_wall_ptr wall2_point_ptr,
            const std::pair<model::wall_border_point, std::optional<model::wall_border_point>>& intersection_pair
        );
    };
}}}


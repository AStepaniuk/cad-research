#pragma once

#include <unordered_map>
#include <map>

#include "floor.h"

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
            corecad::model::vector2d::index_t wall1_free_p;
            corecad::model::vector2d::index_t walls_common_p;
            corecad::model::vector2d::index_t wall2_free_p;

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

        enum class point_on_wall_location { none, start_right, start_left, end_right, end_left };
        struct wall_point_geometry_id
        {
            wall_point_geometry_id(model::wall::index_t w_id, point_on_wall_location w_loc);
            wall_point_geometry_id(model::wall::index_t w1_id, point_on_wall_location w1_loc, model::wall::index_t w2_id, point_on_wall_location w2_loc);

            auto operator<=>(const wall_point_geometry_id&) const = default;

            model::wall::index_t wall1_id;
            point_on_wall_location wall1_location;

            model::wall::index_t wall2_id;
            point_on_wall_location wall2_location;
        };

        struct wall_point_geometry_id_hasher {
            size_t operator()(const domain::plan::calculator::wall_calculator::wall_point_geometry_id& obj) const noexcept {
                size_t h1 = std::hash<model::wall::index_t>{}(obj.wall1_id);
                size_t h2 = std::hash<int>{}(static_cast<int>(obj.wall1_location));
                size_t h3 = std::hash<model::wall::index_t>{}(obj.wall2_id);
                size_t h4 = std::hash<int>{}(static_cast<int>(obj.wall2_location));

                // A common way to combine hashes
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

        struct wall_point_info
        {
            corecad::model::vector2d::index_t index;
            size_t refcount;
        };

        std::unordered_map<wall_point_geometry_id, wall_point_info, wall_point_geometry_id_hasher> _points_cache;

        corecad::model::vector2d::index_t find_or_create_point(
            const wall_point_geometry_id& id,
            const corecad::model::vector2d& p
        );

        void assign_left_intersection_point(
            model::wall& wall1, wall_location wall1_location,
            model::wall& wall2, wall_location wall2_location,
            const corecad::model::vector2d left_intersection_p
        );
    };
}}}


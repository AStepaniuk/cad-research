#pragma once

#include <unordered_map>
#include <map>
#include <iostream>

#include "floor.h"
#include "multi_hash.h"

namespace domain::plan::calculator
{
    class wall_calculator
    {
        model::floor& _floor;

    public:
        wall_calculator(model::floor& floor);

        void recalculate_all_walls();

    private:
        void calculate_stub_wall_start_borders(model::shape::wall& w);
        void calculate_stub_wall_end_borders(model::shape::wall& w);

        struct joined_walls
        {
            model::shape::wall_axis_point::index_t wall1_free_p;
            model::shape::wall_axis_point::index_t walls_common_p;
            model::shape::wall_axis_point::index_t wall2_free_p;

            model::shape::wall_axis_point_locator wall2_apl;
        };

        void recalculate_wall_joints(model::shape::wall& w);
        void calculate_joined_2_walls_borders(
            const model::shape::wall_axis_point_locator& apl,
            std::vector<model::shape::wall_axis_point_locator>& processed_apls
        );
        void calculate_joined_n_walls_borders(
            model::shape::wall_axis_point::index_t apid,
            std::map<model::shape::wall::index_t, double>& walls_directions,
            std::vector<model::shape::wall_axis_point_locator>& processed_apls
        );

        joined_walls get_joined_walls_points(const model::shape::wall_axis_point_locator& apl);

        struct border_point_geometry_id
        {
            border_point_geometry_id(
                model::shape::wall_border_line::index_t b1_id, model::shape::point_on_wall_border_ptr b_p1_ptr,
                model::shape::wall_border_line::index_t b2_id, model::shape::point_on_wall_border_ptr b_p2_ptr
            );

            auto operator<=>(const border_point_geometry_id&) const = default;

            model::shape::wall_border_line::index_t border1_id;
            model::shape::point_on_wall_border_ptr border_point1_ptr;

            model::shape::wall_border_line::index_t border2_id;
            model::shape::point_on_wall_border_ptr border_point2_ptr;
        };

        struct border_point_geometry_id_hasher
        {
            size_t operator()(const domain::plan::calculator::wall_calculator::border_point_geometry_id& obj) const noexcept
            {
                return corecad::util::multi_hash(obj.border1_id, obj.border_point1_ptr, obj.border2_id, obj.border_point2_ptr);
            }
        };

        friend std::ostream& operator<<(std::ostream& os, const border_point_geometry_id& id)
        {
            return os
                << "bid1:" << id.border1_id << " pl1:" << id.border_point1_ptr
                << "bid2:" << id.border2_id << " pl2:" << id.border_point2_ptr;
        }

        struct border_point_info
        {
            model::shape::wall_border_point::index_t index;
            size_t refcount;
        };

        std::unordered_map<border_point_geometry_id, border_point_info, border_point_geometry_id_hasher> _points_cache;

        model::shape::wall_border_point::index_t find_or_create_point(
            const border_point_geometry_id& id,
            const model::shape::wall_border_point& p
        );
        void assign_point_and_borders_to_walls(
            model::shape::wall& w1, model::shape::wall_border_line_ptr b1_ptr, model::shape::point_on_wall_border_ptr p1_ptr,
            model::shape::wall& w2, model::shape::wall_border_line_ptr b2_ptr, model::shape::point_on_wall_border_ptr p2_ptr,
            const model::shape::wall_border_point& point
        );

        struct wall_border_geometry_id
        {
            wall_border_geometry_id(model::shape::wall::index_t w_id, model::shape::wall_border_line_ptr w_b_ptr);

            auto operator<=>(const wall_border_geometry_id&) const = default;

            model::shape::wall::index_t wall_id;
            model::shape::wall_border_line_ptr wall_border_ptr;
        };

        struct wall_border_geometry_id_hasher
        {
            size_t operator()(const domain::plan::calculator::wall_calculator::wall_border_geometry_id& obj) const noexcept
            {
                return corecad::util::multi_hash(obj.wall_id, obj.wall_border_ptr);
            }
        };

        friend std::ostream& operator<<(std::ostream& os, const wall_border_geometry_id& id)
        {
            return os << "wid:" << id.wall_id << " wbl:" << id.wall_border_ptr;
        }

        struct wall_border_info
        {
            model::shape::wall_border_line::index_t index;
            size_t refcount;
        };

        std::unordered_map<wall_border_geometry_id, wall_border_info, wall_border_geometry_id_hasher> _borders_cache;

        model::shape::wall_border_line& find_or_create_border(const wall_border_geometry_id& id);

        void assign_left_intersection_point(
            model::shape::wall& wall1, model::shape::point_on_wall_axis_ptr wall1_location,
            model::shape::wall& wall2, model::shape::point_on_wall_axis_ptr wall2_location,
            const std::pair<model::shape::wall_border_point, std::optional<model::shape::wall_border_point>>& intersection_pair
        );

        void assign_walls_intersection_pair(
            model::shape::wall& wall1, model::shape::wall_border_line_ptr wall1_line_ptr, model::shape::point_on_wall_border_ptr wall1_point_ptr,
            model::shape::wall& wall2, model::shape::wall_border_line_ptr wall2_line_ptr, model::shape::point_on_wall_border_ptr wall2_point_ptr,
            const std::pair<model::shape::wall_border_point, std::optional<model::shape::wall_border_point>>& intersection_pair
        );
    };
}

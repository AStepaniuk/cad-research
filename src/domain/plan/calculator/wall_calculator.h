#pragma once

#include <unordered_map>
#include <map>
#include <iostream>

#include "floor.h"
#include "multi_hash.h"

namespace domain { namespace plan { 
    

namespace model {
    std::ostream& operator<<(std::ostream& os, corecad::model::property<model::wall_border_point::index_t, model::wall_border_line> model::wall_border_line::* pow);
    std::ostream& operator<<(std::ostream& os, model::wall_border_line::index_t model::wall::* pow);    
}

namespace calculator
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
            model::wall_axis_line::index_t axis_id;
            wall_location location;

            auto operator<=>(const wall_finish_id&) const = default;
        };

        friend std::ostream& operator<<(std::ostream& os, wall_location wl)
        {
            return os << (wl == wall_location::start ? 's' : 'e');
        }

        friend std::ostream& operator<<(std::ostream& os, wall_finish_id wfid)
        {
            return os << '{' << wfid.axis_id << ',' << wfid.location << '}';
        }


        using walls_joints = std::map<wall_finish_id, std::vector<wall_finish_id>>;

        struct joined_walls
        {
            model::wall_axis_point::index_t wall1_free_p;
            model::wall_axis_point::index_t walls_common_p;
            model::wall_axis_point::index_t wall2_free_p;

            wall_finish_id wall2_fid;
        };

        void recalculate_wall_joints(model::wall& w, walls_joints& joints);
        void calculate_joined_2_walls_borders(
            wall_finish_id fid,
            const walls_joints& joints,
            const std::map<model::wall_axis_line::index_t, model::wall::index_t>& wall_axis_owners,
            std::vector<wall_finish_id>& processed_fids
        );
        void calculate_joined_n_walls_borders(
            wall_finish_id fid,
            const walls_joints& joints,
            const std::map<model::wall_axis_line::index_t, model::wall::index_t>& wall_axis_owners,
            std::map<model::wall_axis_line::index_t, double>& wall_axis_directions,
            std::vector<wall_finish_id>& processed_fids
        );

        joined_walls get_joined_walls_points(const wall_finish_id& wfid, const walls_joints& joints);

        using point_on_border_ptr =
            corecad::model::property<model::wall_border_point::index_t, model::wall_border_line> model::wall_border_line::*;

        struct border_point_geometry_id
        {
            border_point_geometry_id(
                model::wall_border_line::index_t b1_id, point_on_border_ptr b_p1_ptr,
                model::wall_border_line::index_t b2_id, point_on_border_ptr b_p2_ptr
            );

            auto operator<=>(const border_point_geometry_id&) const = default;

            model::wall_border_line::index_t border1_id;
            point_on_border_ptr border_point1_ptr;

            model::wall_border_line::index_t border2_id;
            point_on_border_ptr border_point2_ptr;
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
            model::wall_border_point::index_t index;
            size_t refcount;
        };

        std::unordered_map<border_point_geometry_id, border_point_info, border_point_geometry_id_hasher> _points_cache;

        model::wall_border_point::index_t find_or_create_point(
            const border_point_geometry_id& id,
            const model::wall_border_point& p
        );
        void assign_point_to_borders(
            model::wall_border_line& b1, point_on_border_ptr p1_ptr,
            model::wall_border_line& b2, point_on_border_ptr p2_ptr,
            const model::wall_border_point& point
        );

        using wall_border_line_ptr = model::wall_border_line::index_t model::wall::*;

        struct wall_border_geometry_id
        {
            wall_border_geometry_id(model::wall::index_t w_id, wall_border_line_ptr w_b_ptr);

            auto operator<=>(const wall_border_geometry_id&) const = default;

            model::wall::index_t wall_id;
            wall_border_line_ptr wall_border_ptr;
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
            model::wall_border_line::index_t index;
            size_t refcount;
        };

        std::unordered_map<wall_border_geometry_id, wall_border_info, wall_border_geometry_id_hasher> _borders_cache;

        model::wall_border_line& find_or_create_border(const wall_border_geometry_id& id);

        void assign_left_intersection_point(
            model::wall& wall1, wall_location wall1_location,
            model::wall& wall2, wall_location wall2_location,
            const std::pair<model::wall_border_point, std::optional<model::wall_border_point>>& intersection_pair
        );

        void assign_walls_intersection_pair(
            model::wall& wall1, wall_border_line_ptr wall1_line_ptr, point_on_border_ptr wall1_point_ptr,
            model::wall& wall2, wall_border_line_ptr wall2_line_ptr, point_on_border_ptr wall2_point_ptr,
            const std::pair<model::wall_border_point, std::optional<model::wall_border_point>>& intersection_pair
        );
    };
}}}

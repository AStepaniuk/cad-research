#include "parameter_calculator.h"

#include <vector>
#include <stdexcept>
#include <algorithm>

using namespace corecad::calculator;
using namespace corecad::model;

namespace
{
    using coord_accessor_f = double&(vector2d&);

    double& access_x(vector2d& p)
    {
        return p.x;
    }

    double& access_y(vector2d& p)
    {
        return p.y;
    }

    void calculate_offsets(
        const parameter_offset& current_offset,
        offset_direction direction,
        coord_accessor_f coord,
        bool reverse,
        const registry<parameter_offset> &parameters,
        registry<vector2d> &points,
        std::vector<vector2d::index_t> &visited
    )
    {
        if (current_offset.direction != direction)
        {
            throw std::runtime_error("Could not calculate offset");
        }

        vector2d& p_from = points.get(reverse ? current_offset.to : current_offset.from);
        vector2d& p_to = points.get(reverse ? current_offset.from : current_offset.to);

        bool is_visited = std::ranges::any_of(
            visited,
            [to_idx = p_to.index](const vector2d::index_t& idx) { return to_idx == idx; }
        );

        if (is_visited)
        {
            throw std::runtime_error("Circular offsets detected");
        }

        coord(p_to) = coord(p_from) + (reverse ? -current_offset.offset : current_offset.offset);
        visited.push_back(p_to.index);

        for(const auto& param : parameters)
        {
            if (param.second.direction != direction)
            {
                continue;
            }

            if (param.second.index == current_offset.index)
            {
                continue;
            }

            if (p_to.index == param.second.from)
            {
                calculate_offsets(
                    param.second,
                    direction,
                    coord,
                    false,
                    parameters,
                    points,
                    visited
                );
            }
            else if (p_to.index == param.second.to)
            {
                calculate_offsets(
                    param.second,
                    direction,
                    coord,
                    true,
                    parameters,
                    points,
                    visited
                );
            }
        }
        
    }

    void do_calulation(
        offset_direction direction,
        coord_accessor_f coord,
        const registry<parameter_offset> &parameters,
        registry<vector2d> &points
    )
    {
        std::vector<vector2d::index_t> visited;
        for(const auto& param : parameters)
        {
            if (param.second.direction != direction)
            {
                continue;
            }

            bool is_processed = std::ranges::any_of(
                visited,
                [&param](const vector2d::index_t& idx) { return param.second.from == idx || param.second.to == idx; }
            );
            if (is_processed)
            {
                continue;
            }

            calculate_offsets(
                param.second,
                direction,
                coord,
                false,
                parameters,
                points,
                visited
            );
        }
    }
}

void corecad::calculator::parameter_calculator::recalculate_all(
    const registry<parameter_offset> &parameters,
    registry<model::vector2d> &points
)
{
    do_calulation(
        offset_direction::horizontal,
        access_x,
        parameters,
        points
    );

    do_calulation(
        offset_direction::vertical,
        access_y,
        parameters,
        points
    );
}
#pragma once

#include <gtest/gtest.h>

#include "wall_calculator.h"
#include "vector2d_assertion.h"

#include <vector>

class wall_calculator_base_fixture : public ::testing::Test
{
protected:
    void given_floor_has_wall_axis_point(const domain::plan::model::wall_axis_point &p);
    void given_floor_has_wall(size_t sp, size_t ep, double w);
    void given_single_wall_floor_generated(
        const domain::plan::model::wall_axis_point& start,
        const domain::plan::model::wall_axis_point& end,
        float width
    );
    void given_two_walls_floor_generated(
        const domain::plan::model::wall_axis_point& start,
        const domain::plan::model::wall_axis_point& common,
        const domain::plan::model::wall_axis_point& end,
        float width1,
        float width2
    );
    void given_recalculating_all_walls();

    using wall_axis_point_index_property_t = corecad::model::property<
        domain::plan::model::wall_axis_point::index_t,
        domain::plan::model::wall
    >;
    void given_wall_point_is_moved_to(
        size_t w,
        wall_axis_point_index_property_t domain::plan::model::wall::*point_definition,
        const domain::plan::model::wall_axis_point& p
    );

    void when_recalculating_all_walls();

    void then_border_points_number_should_be(size_t n);
    void then_border_point_should_be(
        size_t w,
        domain::plan::model::wall_border_point::index_t domain::plan::model::wall::*point_definition,
        const domain::plan::model::wall_border_point& p
    );

    domain::plan::model::floor test_floor;
    domain::plan::calculator::wall_calculator wc { test_floor };

    std::vector<domain::plan::model::wall_axis_point::index_t> points;
    std::vector<domain::plan::model::wall::index_t> walls;
};

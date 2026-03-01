#pragma once

#include <gtest/gtest.h>

#include "wall_calculator.h"
#include "vector2d_assertion.h"

#include <vector>

class wall_calculator_base_fixture : public ::testing::Test
{
protected:
    void given_floor_has_point(const corecad::model::vector2d& p);
    void given_floor_has_wall(size_t sp, size_t ep, double w);
    void given_recalculating_all_walls();
    void given_wall_point_is_moved_to(
        size_t w,
        corecad::model::vector2d::index_t domain::plan::model::wall::*point_definition,
        const corecad::model::vector2d& p
    );

    void when_recalculating_all_walls();

    void then_points_number_should_be(size_t n);
    void then_wall_point_should_be(
        size_t w,
        corecad::model::vector2d::index_t domain::plan::model::wall::*point_definition,
        const corecad::model::vector2d& p
    );

    domain::plan::model::floor test_floor;
    domain::plan::calculator::wall_calculator wc { test_floor };

    std::vector<corecad::model::vector2d::index_t> points;
    std::vector<domain::plan::model::wall::index_t> walls;
};

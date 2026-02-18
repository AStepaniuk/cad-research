#include <gtest/gtest.h>

#include "wall_calculator.h"
#include "vector2d_assertion.h"

using namespace domain::plan::calculator;
using namespace domain::plan::model;
using namespace domain::plan;
using namespace corecad::model;

class when_calculating_wall_borders_for_single_wall : public ::testing::Test {
protected:
    void given_single_wall_floor_generated(vector2d start, vector2d end, float width)
    {
        const auto s = test_floor.points().put(start);
        const auto e = test_floor.points().put(end);

        w = test_floor.walls().make(s, e, width);
    }

    void when_recalculating_all_walls()
    {
        wall_calculator wc { test_floor };
        wc.recalculate_all_walls();
    }

    void then_points_number_should_be(size_t n)
    {
        ASSERT_EQ(test_floor.points().size(), n);
    }

    void then_wall_point_should_be(vector2d::index_t wall::*point_definition, vector2d p)
    {
        const auto& wall = test_floor.walls().get(w);
        const auto& actual_p = test_floor.points().get(wall.*point_definition);

        ASSERT_TRUE(are_vectors_equal(actual_p, p));
    }

    model::floor test_floor;
    wall::index_t w;
};

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stub_points)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_points_number_should_be(6);
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_h_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {1000, 950});
    then_wall_point_should_be(&wall::start_right, {1000, 1050});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_h_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::end_left, {10000, 950});
    then_wall_point_should_be(&wall::end_right, {10000, 1050});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_v_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {1050, 1000});
    then_wall_point_should_be(&wall::start_right, {950, 1000});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_v_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::end_left, {1050, 10000});
    then_wall_point_should_be(&wall::end_right, {950, 10000});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tl_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 10000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {1035.36, 964.64});
    then_wall_point_should_be(&wall::start_right, {964.64, 1035.36});

    then_wall_point_should_be(&wall::end_left, {10035.36, 9964.64});
    then_wall_point_should_be(&wall::end_right, {9964.64, 10035.36});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tr_wall)
{
    given_single_wall_floor_generated({10000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {10035.36, 1035.36});
    then_wall_point_should_be(&wall::start_right, {9964.64, 964.64});

    then_wall_point_should_be(&wall::end_left, {1035.36, 10035.36});
    then_wall_point_should_be(&wall::end_right, {964.64, 9964.64});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_bl_wall)
{
    given_single_wall_floor_generated({1000, 5000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {979.69, 4954.31});
    then_wall_point_should_be(&wall::start_right, {1020.31, 5045.69});

    then_wall_point_should_be(&wall::end_left, {9979.69, 954.31});
    then_wall_point_should_be(&wall::end_right, {10020.31, 1045.69});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_br_wall)
{
    given_single_wall_floor_generated({10000, 5000}, {1000, 1000}, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(&wall::start_left, {9979.69, 5045.69});
    then_wall_point_should_be(&wall::start_right, {10020.31, 4954.31});

    then_wall_point_should_be(&wall::end_left, {979.69, 1045.69});
    then_wall_point_should_be(&wall::end_right, {1020.31, 954.31});
}

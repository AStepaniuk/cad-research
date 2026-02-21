#include <gtest/gtest.h>

#include "wall_calculator.h"
#include "vector2d_assertion.h"

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_two_walls : public ::testing::Test
{
protected:
    void given_two_walls_floor_generated(vector2d start, vector2d common, vector2d end, float width1, float width2)
    {
        const auto s = test_floor.points().put(start);
        const auto c = test_floor.points().put(common);
        const auto e = test_floor.points().put(end);

        w1 = test_floor.walls().make(s, c, width1);
        w2 = test_floor.walls().make(c, e, width2);
    }

    void given_recalculating_all_walls()
    {
        wc.recalculate_all_walls();
    }

    void given_wall_point_is_moved_to(wall::index_t w, vector2d::index_t wall::*point_definition, vector2d p)
    {
        const auto& wall = test_floor.walls().get(w);
        auto& point = test_floor.points().get(wall.*point_definition);

        point.x = p.x;
        point.y = p.y;
    }

    void when_recalculating_all_walls()
    {
        wc.recalculate_all_walls();
    }

    void then_points_number_should_be(size_t n)
    {
        ASSERT_EQ(test_floor.points().size(), n);
    }

    void then_wall_point_should_be(wall::index_t w, vector2d::index_t wall::*point_definition, vector2d p)
    {
        const auto& wall = test_floor.walls().get(w);
        const auto& actual_p = test_floor.points().get(wall.*point_definition);

        ASSERT_TRUE(are_vectors_equal(actual_p, p));
    }

    domain::plan::model::floor test_floor;
    wall_calculator wc { test_floor };

    wall::index_t w1;
    wall::index_t w2;
};

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_duplicate_corner_points)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_points_number_should_be(9);
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tl_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000, 10000}, {1000, 1000}, {10000, 1000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {950, 950});
    then_wall_point_should_be(w1, &wall::end_right, {1050, 1050});

    then_wall_point_should_be(w2, &wall::start_left, {950, 950});
    then_wall_point_should_be(w2, &wall::start_right, {1050, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tr_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {10050, 950});
    then_wall_point_should_be(w1, &wall::end_right, {9950, 1050});

    then_wall_point_should_be(w2, &wall::start_left, {10050, 950});
    then_wall_point_should_be(w2, &wall::start_right, {9950, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_br_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000, 1000}, {10000, 10000}, {1000, 10000}, 100, 200);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {10050, 10100});
    then_wall_point_should_be(w1, &wall::end_right, {9950, 9900});

    then_wall_point_should_be(w2, &wall::start_left, {10050, 10100});
    then_wall_point_should_be(w2, &wall::start_right, {9950, 9900});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_bl_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000, 10000}, {1000, 10000}, {1000, 1000}, 200, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {950, 10100});
    then_wall_point_should_be(w1, &wall::end_right, {1050, 9900});

    then_wall_point_should_be(w2, &wall::start_left, {950, 10100});
    then_wall_point_should_be(w2, &wall::start_right, {1050, 9900});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_v_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {1000, 5000}, {1000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {1050, 5000});
    then_wall_point_should_be(w1, &wall::end_right, {950, 5000});

    then_wall_point_should_be(w2, &wall::start_left, {1050, 5000});
    then_wall_point_should_be(w2, &wall::start_right, {950, 5000});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_h_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {5000, 1000}, {10000, 1000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {5000, 950});
    then_wall_point_should_be(w1, &wall::end_right, {5000, 1050});

    then_wall_point_should_be(w2, &wall::start_left, {5000, 950});
    then_wall_point_should_be(w2, &wall::start_right, {5000, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tl_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {3000, 5500}, {5000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {3045.69, 5479.69});
    then_wall_point_should_be(w1, &wall::end_right, {2954.31, 5520.31});

    then_wall_point_should_be(w2, &wall::start_left, {3045.69, 5479.69});
    then_wall_point_should_be(w2, &wall::start_right, {2954.31, 5520.31});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tr_same_walls_width)
{
    given_two_walls_floor_generated({5000, 1000}, {3000, 5500}, {1000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_wall_point_should_be(w1, &wall::end_left, {3045.69, 5520.31});
    then_wall_point_should_be(w1, &wall::end_right, {2954.31, 5479.69});

    then_wall_point_should_be(w2, &wall::start_left, {3045.69, 5520.31});
    then_wall_point_should_be(w2, &wall::start_right, {2954.31, 5479.69});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_add_points_after_second_calculation)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);
    given_recalculating_all_walls();
    given_wall_point_is_moved_to(w1, &wall::end, {2000, 2000});

    when_recalculating_all_walls();

    then_points_number_should_be(9);
}

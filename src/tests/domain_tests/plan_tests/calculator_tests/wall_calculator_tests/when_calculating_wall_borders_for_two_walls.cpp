#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "vector2d_assertion.h"

using namespace domain::plan::model::shape;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_two_walls : public wall_calculator_base_fixture { };

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_duplicate_corner_points)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_border_points_number_should_be(6);
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tl_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000, 10000}, {1000, 1000}, {10000, 1000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {950, 950});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {1050, 1050});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {950, 950});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {1050, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tr_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10050, 950});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {9950, 1050});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {10050, 950});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {9950, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_br_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000, 1000}, {10000, 10000}, {1000, 10000}, 100, 200);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10050, 10100});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {9950, 9900});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {10050, 10100});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {9950, 9900});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_bl_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000, 10000}, {1000, 10000}, {1000, 1000}, 200, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {950, 10100});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {1050, 9900});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {950, 10100});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {1050, 9900});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_v_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {1000, 5000}, {1000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {1050, 5000});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {950, 5000});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {1050, 5000});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {950, 5000});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_h_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {5000, 1000}, {10000, 1000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {5000, 950});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {5000, 1050});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {5000, 950});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {5000, 1050});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tl_same_walls_width)
{
    given_two_walls_floor_generated({1000, 1000}, {3000, 5500}, {5000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {3045.69, 5479.69});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {2954.31, 5520.31});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {3045.69, 5479.69});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {2954.31, 5520.31});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tr_same_walls_width)
{
    given_two_walls_floor_generated({5000, 1000}, {3000, 5500}, {1000, 10000}, 100, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {3045.69, 5520.31});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {2954.31, 5479.69});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {3045.69, 5520.31});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {2954.31, 5479.69});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_add_points_after_second_calculation)
{
    given_two_walls_floor_generated({1000, 1000}, {10000, 1000}, {10000, 10000}, 100, 100);
    given_recalculating_all_walls();
    std::cout << test_floor.data().items<wall>();
    std::cout << test_floor.data().items<wall_axis_line>();
    std::cout << test_floor.data().items<wall_axis_point>();
    std::cout << test_floor.data().items<wall_border_line>();

    given_wall_point_is_moved_to(0, &wall_axis_line::e, {2000, 2000});

    when_recalculating_all_walls();

    std::cout << test_floor.data().items<wall>();
    std::cout << test_floor.data().items<wall_axis_line>();
    std::cout << test_floor.data().items<wall_axis_point>();
    std::cout << test_floor.data().items<wall_border_line>();

    then_border_points_number_should_be(6);
}

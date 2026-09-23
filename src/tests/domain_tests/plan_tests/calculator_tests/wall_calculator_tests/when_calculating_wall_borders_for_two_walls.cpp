#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "point2d_assertion.h"

using namespace domain::plan::model::shape;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_two_walls : public wall_calculator_base_fixture { };

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_duplicate_corner_points)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_points_number_should_be(6);
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tl_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000.0 * mm, 10000.0 * mm}, {1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {950.0 * mm, 950.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {1050.0 * mm, 1050.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {950.0 * mm, 950.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {1050.0 * mm, 1050.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_tr_corner_same_walls_width)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10050.0 * mm, 950.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {9950.0 * mm, 1050.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {10050.0 * mm, 950.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {9950.0 * mm, 1050.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_br_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 10000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm, 200.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10050.0 * mm, 10100.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {9950.0 * mm, 9900.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {10050.0 * mm, 10100.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {9950.0 * mm, 9900.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_bl_corner_different_walls_width)
{
    given_two_walls_floor_generated({10000.0 * mm, 10000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, {1000.0 * mm, 1000.0 * mm}, 200.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {950.0 * mm, 10100.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {1050.0 * mm, 9900.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {950.0 * mm, 10100.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {1050.0 * mm, 9900.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_v_same_walls_width)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {1000.0 * mm, 5000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {1050.0 * mm, 5000.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {950.0 * mm, 5000.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {1050.0 * mm, 5000.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {950.0 * mm, 5000.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_h_same_walls_width)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {5000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {5000.0 * mm, 950.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {5000.0 * mm, 1050.0 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {5000.0 * mm, 950.0 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {5000.0 * mm, 1050.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tl_same_walls_width)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {3000.0 * mm, 5500.0 * mm}, {5000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {3045.69 * mm, 5479.69 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {2954.31 * mm, 5520.31 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {3045.69 * mm, 5479.69 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {2954.31 * mm, 5520.31 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_generate_straight_joint_diag_tr_same_walls_width)
{
    given_two_walls_floor_generated({5000.0 * mm, 1000.0 * mm}, {3000.0 * mm, 5500.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {3045.69 * mm, 5520.31 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {2954.31 * mm, 5479.69 * mm});

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, {3045.69 * mm, 5520.31 * mm});
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, {2954.31 * mm, 5479.69 * mm});
}

TEST_F(when_calculating_wall_borders_for_two_walls, should_not_add_points_after_second_calculation)
{
    given_two_walls_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 10000.0 * mm}, 100.0 * mm, 100.0 * mm);
    given_recalculating_all_walls();

    given_wall_point_is_moved_to(0, &wall_axis_line::e, {2000.0 * mm, 2000.0 * mm});

    when_recalculating_all_walls();

    then_border_points_number_should_be(6);
}

#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "vector2d_assertion.h"

using namespace domain::plan::calculator;
using namespace domain::plan::model;
using namespace domain::plan;
using namespace corecad::model;

class when_calculating_wall_borders_for_single_wall : public wall_calculator_base_fixture { };

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stub_points)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_border_points_number_should_be(4);
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_h_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {1000, 950});
    then_border_point_should_be(0, &wall::start_right, {1000, 1050});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_h_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::end_left, {10000, 950});
    then_border_point_should_be(0, &wall::end_right, {10000, 1050});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_v_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {1050, 1000});
    then_border_point_should_be(0, &wall::start_right, {950, 1000});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_v_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::end_left, {1050, 10000});
    then_border_point_should_be(0, &wall::end_right, {950, 10000});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tl_wall)
{
    given_single_wall_floor_generated({1000, 1000}, {10000, 10000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {1035.36, 964.64});
    then_border_point_should_be(0, &wall::start_right, {964.64, 1035.36});

    then_border_point_should_be(0, &wall::end_left, {10035.36, 9964.64});
    then_border_point_should_be(0, &wall::end_right, {9964.64, 10035.36});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tr_wall)
{
    given_single_wall_floor_generated({10000, 1000}, {1000, 10000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {10035.36, 1035.36});
    then_border_point_should_be(0, &wall::start_right, {9964.64, 964.64});

    then_border_point_should_be(0, &wall::end_left, {1035.36, 10035.36});
    then_border_point_should_be(0, &wall::end_right, {964.64, 9964.64});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_bl_wall)
{
    given_single_wall_floor_generated({1000, 5000}, {10000, 1000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {979.69, 4954.31});
    then_border_point_should_be(0, &wall::start_right, {1020.31, 5045.69});

    then_border_point_should_be(0, &wall::end_left, {9979.69, 954.31});
    then_border_point_should_be(0, &wall::end_right, {10020.31, 1045.69});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_br_wall)
{
    given_single_wall_floor_generated({10000, 5000}, {1000, 1000}, 100);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::start_left, {9979.69, 5045.69});
    then_border_point_should_be(0, &wall::start_right, {10020.31, 4954.31});

    then_border_point_should_be(0, &wall::end_left, {979.69, 1045.69});
    then_border_point_should_be(0, &wall::end_right, {1020.31, 954.31});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_not_add_points_after_second_calculation)
{
    given_single_wall_floor_generated({10000, 5000}, {1000, 1000}, 100);
    given_recalculating_all_walls();
    given_wall_point_is_moved_to(0, &wall::start, {2000, 2000});

    when_recalculating_all_walls();

    then_border_points_number_should_be(4);
}

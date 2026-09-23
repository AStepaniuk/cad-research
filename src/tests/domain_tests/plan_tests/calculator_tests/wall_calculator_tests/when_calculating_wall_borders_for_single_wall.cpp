#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "point2d_assertion.h"

using namespace domain::plan::calculator;
using namespace domain::plan::model::shape;
using namespace domain::plan;
using namespace corecad::model;

class when_calculating_wall_borders_for_single_wall : public wall_calculator_base_fixture { };

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stub_points)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_points_number_should_be(4);
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_h_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {1000.0 * mm, 950.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {1000.0 * mm, 1050.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_h_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10000.0 * mm, 950.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {10000.0 * mm, 1050.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_start_stub_v_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {1050.0 * mm, 1000.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {950.0 * mm, 1000.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_end_stub_v_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {1050.0 * mm, 10000.0 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {950.0 * mm, 10000.0 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tl_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 1000.0 * mm}, {10000.0 * mm, 10000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {1035.36 * mm, 964.64 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {964.64 * mm, 1035.36 * mm});

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {10035.36 * mm, 9964.64 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {9964.64 * mm, 10035.36 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_tr_wall)
{
    given_single_wall_floor_generated({10000.0 * mm, 1000.0 * mm}, {1000.0 * mm, 10000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {10035.36 * mm, 1035.36 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {9964.64 * mm, 964.64 * mm});

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {1035.36 * mm, 10035.36 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {964.64 * mm, 9964.64 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_bl_wall)
{
    given_single_wall_floor_generated({1000.0 * mm, 5000.0 * mm}, {10000.0 * mm, 1000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {979.69 * mm, 4954.31 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {1020.31 * mm, 5045.69 * mm});

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {9979.69 * mm, 954.31 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {10020.31 * mm, 1045.69 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_generate_stubs_diag_br_wall)
{
    given_single_wall_floor_generated({10000.0 * mm, 5000.0 * mm}, {1000.0 * mm, 1000.0 * mm}, 100.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, {9979.69 * mm, 5045.69 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, {10020.31 * mm, 4954.31 * mm});

    then_border_point_should_be(0, &wall::left, &wall_border_line::e, {979.69 * mm, 1045.69 * mm});
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, {1020.31 * mm, 954.31 * mm});
}

TEST_F(when_calculating_wall_borders_for_single_wall, should_not_add_points_after_second_calculation)
{
    given_single_wall_floor_generated({10000.0 * mm, 5000.0 * mm}, {1000.0 * mm, 1000.0 * mm}, 100.0 * mm);
    given_recalculating_all_walls();
    given_wall_point_is_moved_to(0, &wall_axis_line::s, {2000.0 * mm, 2000.0 * mm});

    when_recalculating_all_walls();

    then_border_points_number_should_be(4);
}

#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "point2d_assertion.h"

#include <vector>

using namespace domain::plan::model::shape;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_added_wall : public wall_calculator_base_fixture { };

TEST_F(when_calculating_wall_borders_for_added_wall, should_calculate_added_wall)
{
    given_floor_has_wall_axis_point({1000.0 * mm, 1000.0 * mm});
    given_floor_has_wall_axis_point({5000.0 * mm, 1000.0 * mm});
    given_floor_has_wall_axis_point({10000.0 * mm, 1000.0 * mm});

    given_floor_has_wall(0, 1, 100.0 * mm);
    given_floor_has_wall(1, 2, 100.0 * mm);

    given_recalculating_all_walls();

    given_floor_has_wall_axis_point({5000.0 * mm, 5000.0 * mm});
    given_floor_has_wall(1, 3, 200.0 * mm);

    when_recalculating_all_walls();

    then_border_points_number_should_be(9);
}

TEST_F(when_calculating_wall_borders_for_added_wall, should_calculate_added_wall_borders)
{
    given_floor_has_wall_axis_point({1000.0 * mm, 1000.0 * mm});
    given_floor_has_wall_axis_point({5000.0 * mm, 1000.0 * mm});
    given_floor_has_wall_axis_point({10000.0 * mm, 1000.0 * mm});

    given_floor_has_wall(0, 1, 100.0 * mm);
    given_floor_has_wall(1, 2, 100.0 * mm);

    given_recalculating_all_walls();

    given_floor_has_wall_axis_point({5000.0 * mm, 5000.0 * mm});
    given_floor_has_wall(1, 3, 200.0 * mm);

    when_recalculating_all_walls();

    then_border_point_should_be(0, &wall::left, &wall_border_line::s, { 1000.0 * mm, 950.0 * mm });
    then_border_point_should_be(0, &wall::right, &wall_border_line::s, { 1000.0 * mm, 1050.0 * mm });
    then_border_point_should_be(0, &wall::left, &wall_border_line::e, { 5000.0 * mm, 950.0 * mm });
    then_border_point_should_be(0, &wall::right, &wall_border_line::e, { 4900.0 * mm, 1050.0 * mm });

    then_border_point_should_be(1, &wall::left, &wall_border_line::s, { 5000.0 * mm, 950.0 * mm });
    then_border_point_should_be(1, &wall::right, &wall_border_line::s, { 5100.0 * mm, 1050.0 * mm });
    then_border_point_should_be(1, &wall::left, &wall_border_line::e, { 10000.0 * mm, 950.0 * mm });
    then_border_point_should_be(1, &wall::right, &wall_border_line::e, { 10000.0 * mm, 1050.0 * mm });

    then_border_point_should_be(2, &wall::left, &wall_border_line::s, { 5100.0 * mm, 1050.0 * mm });
    then_border_point_should_be(2, &wall::right, &wall_border_line::s, { 4900.0 * mm, 1050.0 * mm });
    then_border_point_should_be(2, &wall::left, &wall_border_line::e, { 5100.0 * mm, 5000.0 * mm });
    then_border_point_should_be(2, &wall::right, &wall_border_line::e, { 4900.0 * mm, 5000.0 * mm });
}


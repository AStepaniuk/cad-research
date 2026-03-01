#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "vector2d_assertion.h"

#include <vector>

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_added_wall : public wall_calculator_base_fixture
{
};

TEST_F(when_calculating_wall_borders_for_added_wall, should_calculate_added_wall)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);

    given_recalculating_all_walls();

    given_floor_has_point({5000, 5000});
    given_floor_has_wall(1, 3, 200);

    when_recalculating_all_walls();

    then_points_number_should_be(13);
}

TEST_F(when_calculating_wall_borders_for_added_wall, should_calculate_added_wall_borders)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);

    given_recalculating_all_walls();

    given_floor_has_point({5000, 5000});
    given_floor_has_wall(1, 3, 200);

    when_recalculating_all_walls();

    then_wall_point_should_be(0, &wall::start_left, { 1000, 950 });
    then_wall_point_should_be(0, &wall::start_right, { 1000, 1050 });
    then_wall_point_should_be(0, &wall::end_left, { 5000, 950 });
    then_wall_point_should_be(0, &wall::end_right, { 4900, 1050 });

    then_wall_point_should_be(1, &wall::start_left, { 5000, 950 });
    then_wall_point_should_be(1, &wall::start_right, { 5100, 1050 });
    then_wall_point_should_be(1, &wall::end_left, { 10000, 950 });
    then_wall_point_should_be(1, &wall::end_right, { 10000, 1050 });

    then_wall_point_should_be(2, &wall::start_left, { 5100, 1050 });
    then_wall_point_should_be(2, &wall::start_right, { 4900, 1050 });
    then_wall_point_should_be(2, &wall::end_left, { 5100, 5000 });
    then_wall_point_should_be(2, &wall::end_right, { 4900, 5000 });
}


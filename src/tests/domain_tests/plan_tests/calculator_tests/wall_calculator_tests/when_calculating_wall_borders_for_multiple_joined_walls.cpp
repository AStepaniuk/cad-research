#include "wall_calculator_base_fixture.h"

#include "wall_calculator.h"
#include "vector2d_assertion.h"

#include <vector>

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_multiple_joined_walls : public wall_calculator_base_fixture
{
};

TEST_F(when_calculating_wall_borders_for_multiple_joined_walls, should_not_duplicate_corner_points)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});
    given_floor_has_point({5000, 5000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);
    given_floor_has_wall(1, 3, 100);

    when_recalculating_all_walls();

    then_points_number_should_be(13);
}

TEST_F(when_calculating_wall_borders_for_multiple_joined_walls, should_calculate_corner_points)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});
    given_floor_has_point({5000, 5000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);
    given_floor_has_wall(1, 3, 200);

    when_recalculating_all_walls();

    then_wall_point_should_be(0, &wall::end_left, { 5000, 950 });
    then_wall_point_should_be(0, &wall::end_right, { 4900, 1050 });

    then_wall_point_should_be(1, &wall::start_left, { 5000, 950 });
    then_wall_point_should_be(1, &wall::start_right, { 5100, 1050 });

    then_wall_point_should_be(2, &wall::start_left, { 5100, 1050 });
    then_wall_point_should_be(2, &wall::start_right, { 4900, 1050 });
}

TEST_F(when_calculating_wall_borders_for_multiple_joined_walls, should_not_add_points_after_second_calculation)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});
    given_floor_has_point({5000, 5000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);
    given_floor_has_wall(1, 3, 200);

    given_recalculating_all_walls();
    given_wall_point_is_moved_to(0, &wall::start, {2000, 2000});

    when_recalculating_all_walls();

    then_points_number_should_be(13);
}

TEST_F(when_calculating_wall_borders_for_multiple_joined_walls, should_not_add_points_after_second_calculation_when_walls_are_rearranged)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});
    given_floor_has_point({5000, 5000});

    given_floor_has_wall(0, 1, 100);
    given_floor_has_wall(1, 2, 100);
    given_floor_has_wall(1, 3, 200);

    given_recalculating_all_walls();
    given_wall_point_is_moved_to(1, &wall::end, {5000, 5000});
    given_wall_point_is_moved_to(2, &wall::end, {10000, 5000});

    when_recalculating_all_walls();

    then_points_number_should_be(13);
}


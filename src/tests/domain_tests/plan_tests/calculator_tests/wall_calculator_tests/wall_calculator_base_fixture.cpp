#include "wall_calculator_base_fixture.h"

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

void wall_calculator_base_fixture::given_floor_has_wall_axis_point(const wall_axis_point &p)
{
    points.push_back(test_floor.data().put(p));
}

void wall_calculator_base_fixture::given_floor_has_wall(size_t sp, size_t ep, double w)
{
    walls.push_back(test_floor.data().make<wall>(points[sp], points[ep], w));
}

void wall_calculator_base_fixture::given_single_wall_floor_generated(
    const domain::plan::model::wall_axis_point &start,
    const domain::plan::model::wall_axis_point &end,
    float width
)
{
    given_floor_has_wall_axis_point(start);
    given_floor_has_wall_axis_point(end);

    given_floor_has_wall(0, 1, width);
}

void wall_calculator_base_fixture::given_two_walls_floor_generated(
    const domain::plan::model::wall_axis_point &start,
    const domain::plan::model::wall_axis_point &common,
    const domain::plan::model::wall_axis_point &end,
    float width1,
    float width2
)
{
    given_floor_has_wall_axis_point(start);
    given_floor_has_wall_axis_point(common);
    given_floor_has_wall_axis_point(end);

    given_floor_has_wall(0, 1, width1);
    given_floor_has_wall(1, 2, width2);
}

void wall_calculator_base_fixture::given_recalculating_all_walls()
{
    wc.recalculate_all_walls();
}

void wall_calculator_base_fixture::given_wall_point_is_moved_to(
    size_t w,
    wall_axis_point_index_property_t wall::*point_definition,
    const wall_axis_point& p
)
{
    const auto& wall = test_floor.data().get(walls[w]);
    auto& point = test_floor.data().get(wall.*point_definition);

    point.x = p.x;
    point.y = p.y;
}

void wall_calculator_base_fixture::when_recalculating_all_walls()
{
    wc.recalculate_all_walls();
}

void wall_calculator_base_fixture::then_border_points_number_should_be(size_t n)
{
    ASSERT_EQ(test_floor.data().size<wall_border_point>(), n);
}

void wall_calculator_base_fixture::then_border_point_should_be(
    size_t w,
    wall_border_point::index_t wall::*point_definition,
    const wall_border_point &p
)
{
    const auto& wall = test_floor.data().get(walls[w]);
    const auto& actual_p = test_floor.data().get(wall.*point_definition);

    ASSERT_TRUE(are_vectors_equal(actual_p, p));
}

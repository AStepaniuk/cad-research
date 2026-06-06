#include "wall_calculator_base_fixture.h"

using namespace domain::plan::model::shape;
using namespace domain::plan::calculator;
using namespace corecad::model;

void wall_calculator_base_fixture::given_floor_has_wall_axis_point(const wall_axis_point &p)
{
    points.push_back(test_floor.data().put(p));
}

void wall_calculator_base_fixture::given_floor_has_wall(size_t sp, size_t ep, double w)
{
    const auto a = test_floor.data().make<wall_axis_line>(points[sp], points[ep]);
    walls.push_back(test_floor.data().make<wall>(a, w));
}

void wall_calculator_base_fixture::given_single_wall_floor_generated(
    const wall_axis_point &start,
    const wall_axis_point &end,
    float width
)
{
    given_floor_has_wall_axis_point(start);
    given_floor_has_wall_axis_point(end);

    given_floor_has_wall(0, 1, width);
}

void wall_calculator_base_fixture::given_two_walls_floor_generated(
    const wall_axis_point &start,
    const wall_axis_point &common,
    const wall_axis_point &end,
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
    wall_axis_point_index_property_t wall_axis_line::*point_definition,
    const wall_axis_point& p
)
{
    const auto& wall = test_floor.data().get(walls[w]);
    const auto& axis = test_floor.data().get(wall.axis);
    auto& point = test_floor.data().get(axis.*point_definition);

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
    wall_border_line::index_t wall::*border_definition,
    point_on_border_ptr point_definition,
    const wall_border_point& p
)
{
    const auto& wall = test_floor.data().get(walls[w]);

    const auto& border = test_floor.data().get(wall.*border_definition);
    const auto& actual_p = test_floor.data().get(border.*point_definition);

    ASSERT_TRUE(are_vectors_equal(actual_p, p));
}

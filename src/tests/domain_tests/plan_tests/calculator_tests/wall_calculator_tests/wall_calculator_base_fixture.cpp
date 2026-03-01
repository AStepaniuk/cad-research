#include "wall_calculator_base_fixture.h"

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

void wall_calculator_base_fixture::given_floor_has_point(const vector2d &p)
{
    points.push_back(test_floor.points().put(p));
}

void wall_calculator_base_fixture::given_floor_has_wall(size_t sp, size_t ep, double w)
{
    walls.push_back(test_floor.walls().make(points[sp], points[ep], w));
}

void wall_calculator_base_fixture::given_recalculating_all_walls()
{
    wc.recalculate_all_walls();
}

void wall_calculator_base_fixture::given_wall_point_is_moved_to(
    size_t w,
    vector2d::index_t wall::*point_definition,
    const vector2d& p
)
{
    const auto& wall = test_floor.walls().get(walls[w]);
    auto& point = test_floor.points().get(wall.*point_definition);

    point.x = p.x;
    point.y = p.y;
}

void wall_calculator_base_fixture::when_recalculating_all_walls()
{
    wc.recalculate_all_walls();
}

void wall_calculator_base_fixture::then_points_number_should_be(size_t n)
{
    ASSERT_EQ(test_floor.points().size(), n);
}

void wall_calculator_base_fixture::then_wall_point_should_be(
    size_t w,
    vector2d::index_t wall::*point_definition,
    const vector2d &p
)
{
    const auto& wall = test_floor.walls().get(walls[w]);
    const auto& actual_p = test_floor.points().get(wall.*point_definition);

    ASSERT_TRUE(are_vectors_equal(actual_p, p));
}

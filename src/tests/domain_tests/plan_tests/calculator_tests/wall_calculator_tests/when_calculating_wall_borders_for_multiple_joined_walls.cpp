#include <gtest/gtest.h>

#include "wall_calculator.h"
#include "vector2d_assertion.h"

#include <vector>

using namespace domain::plan::model;
using namespace domain::plan::calculator;
using namespace corecad::model;

class when_calculating_wall_borders_for_multiple_joined_walls : public ::testing::Test {
protected:
    void given_floor_has_point(vector2d p)
    {
        points.push_back(test_floor.points().put(p));
    }
    void given_floor_has_wall(size_t sp, size_t ep, double w)
    {
        walls.push_back(test_floor.walls().make(points[sp], points[ep], w));
    }

    void when_recalculating_all_walls()
    {
        wall_calculator wc;
        wc.recalculate_all_walls(test_floor);
    }

    void then_points_number_should_be(size_t n)
    {
        ASSERT_EQ(test_floor.points().size(), n);
    }

    void then_wall_point_should_be(wall::index_t w, vector2d::index_t wall::*point_definition, vector2d p)
    {
        const auto& wall = test_floor.walls().get(w);
        const auto& actual_p = test_floor.points().get(wall.*point_definition);

        ASSERT_TRUE(are_vectors_equal(actual_p, p));
    }

    floor test_floor;

    std::vector<vector2d::index_t> points;
    std::vector<wall::index_t> walls;
};

TEST_F(when_calculating_wall_borders_for_multiple_joined_walls, should_not_duplicate_corner_points)
{
    given_floor_has_point({1000, 1000});
    given_floor_has_point({5000, 1000});
    given_floor_has_point({10000, 1000});
    given_floor_has_point({5000, 5000});

    given_floor_has_wall(1, 2, 100);
    given_floor_has_wall(2, 3, 100);
    given_floor_has_wall(2, 4, 100);

    when_recalculating_all_walls();

    then_points_number_should_be(13);
}


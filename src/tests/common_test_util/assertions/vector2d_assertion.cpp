#include "vector2d_assertion.h"

using namespace corecad::model;

::testing::AssertionResult are_vectors_equal(const vector2d& actual, const vector2d& expected, double tolerance)
{
    const auto t2 = tolerance * tolerance;

    const auto dx = actual.x - expected.x;
    const auto dy = actual.y - expected.y;

    if (dx * dx + dy * dy < t2) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure()
            << "Actual vector (" << actual.x << "," << actual.y << ") does not match to the expected: ("
            << expected.x << "," << expected.y << ")";
    }
}
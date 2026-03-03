#pragma once

#include <gtest/gtest.h>

#include "vector2d.h"

template<typename TVectorTag>
::testing::AssertionResult are_vectors_equal(
    const corecad::model::vector2d<TVectorTag>& actual,
    const corecad::model::vector2d<TVectorTag>& expected,
    double tolerance = 0.01
)
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

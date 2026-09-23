#pragma once

#include <gtest/gtest.h>

#include "point2d.h"

template<typename TVectorTag>
::testing::AssertionResult are_points_equal(
    const corecad::model::point2d<TVectorTag>& actual,
    const corecad::model::point2d<TVectorTag>& expected,
    corecad::model::length_mm_t tolerance = 0.01 * corecad::model::mm
)
{
    const auto t2 = tolerance * tolerance;

    const auto dx = actual.x.val() - expected.x.val();
    const auto dy = actual.y.val() - expected.y.val();

    if (dx * dx + dy * dy < t2) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure()
            << "Actual vector (" << actual.x << "," << actual.y << ") does not match to the expected: ("
            << expected.x << "," << expected.y << ")";
    }
}

#pragma once

#include <gtest/gtest.h>

#include "vector2d.h"

::testing::AssertionResult are_vectors_equal(
    const corecad::model::vector2d& actual,
    const corecad::model::vector2d& expected,
    double tolerance = 0.01
);

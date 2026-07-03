#pragma once

#include <iostream>

namespace corecad::model
{
    enum class coordinate2d { x, y };

    inline std::ostream& operator<<(std::ostream& os, coordinate2d c)
    {
        return os << (c == coordinate2d::x ? "x" : "y");
    }
}
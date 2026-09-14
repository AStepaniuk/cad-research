#pragma once

#include <ostream>
#include <vector>

namespace std
{
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
    {
        bool first = true;
        for (const auto& i : v)
        {
            if (!first) os << " | ";
            os << i;

            first = false;
        }

        return os;
    }
}

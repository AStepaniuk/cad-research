#pragma once

#include <variant>

#include "wall_border_point_locator.h"

namespace domain::plan::model::parameter
{
    using point_locator_t = std::variant<wall_border_point_locator>;

    std::ostream& operator<<(std::ostream& os, const point_locator_t& l)
    {
        std::visit([&os](auto& impl) {
            os << impl;
        }, l);

        return os;
    }
}

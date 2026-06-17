#pragma once

#include <variant>

#include "floor.h"

namespace domain::plan::resolver
{
    class point_resolver
    {
        const model::floor &_floor;

        public:
            using point_index_t = std::variant<
                model::shape::wall_border_point::index_t
            >;

            point_resolver(const model::floor &floor);

            point_index_t resolve(const model::parameter::point_locator_t& pl);
    };
}

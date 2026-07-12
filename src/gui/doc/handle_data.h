#pragma once

#include <vector>
#include <iostream>

#include "parameter.h"
#include "floor.h"

namespace gui::doc {
    class handle_data
    {
    public:
        using point_locator_t = domain::plan::model::parameter::point_locator_t;
        using point_id_t = domain::plan::model::shape::wall_points_ids_tl::variant_t;

        handle_data(point_id_t id)
            : _handle_locators{ }
            , _handle_id{ std::move(id) }
        {}

        handle_data(std::vector<point_locator_t> vl, point_id_t id)
            : _handle_locators{ std::move(vl) }
            , _handle_id{ std::move(id) }
        {}

        const std::vector<point_locator_t>& handle_locators() const noexcept
        {
            return _handle_locators;
        }

        const point_id_t& handle_id() const noexcept
        {
            return _handle_id;
        }

        template <typename TPoint>
        requires (corecad::util::IsOneOf<TPoint, domain::plan::model::shape::wall_points_tl>)
        typename TPoint::index_t handle_id_of_type() const
        {
            const auto* hid_ptr = std::get_if<typename TPoint::index_t>(&_handle_id);
            return hid_ptr ? *hid_ptr : typename TPoint::index_t {};
        }

    private:
        // empty if handle point has no locator yet
        std::vector<point_locator_t> _handle_locators;
        point_id_t _handle_id;
    };

    std::ostream& operator<<(std::ostream& os, const handle_data& hd);
}

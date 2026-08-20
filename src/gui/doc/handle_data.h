#pragma once

#include <vector>
#include <iostream>

#include "parameter.h"
#include "floor.h"
#include "overloaded.h"

namespace gui::doc
{
    class handle_data
    {
    public:
        using point_id_t = domain::plan::model::shape::wall_points_ids_tl::variant_t;

        handle_data(point_id_t id)
            : _handle_id{ std::move(id) }
        {}

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
        point_id_t _handle_id;
    };

    std::ostream& operator<<(std::ostream& os, const handle_data& hd);
}

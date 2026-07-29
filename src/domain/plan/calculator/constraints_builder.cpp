#include "constraints_builder.h"

#include "overloaded.h"
#include "views_take_model_variants.h"

using namespace domain::plan::calculator;
using namespace domain::plan;
using namespace domain::plan::model::shape;
using namespace domain::plan::model::parameter;
using namespace corecad::model;
using namespace corecad::model::constraint;

constraints_builder::constraints_builder(model::floor &floor, resolver::point_resolver& pr)
    : _floor { floor }
    , _pr { pr }
{
}

void constraints_builder::rebuild_all_constraints()
{
    // TODO: implement smart GC here later.
    // for now just clear and rebuild everything
    _floor.data().clear<model::floor::constraint_t>();

    // generate parameters-based constraints
    for (const auto& pair : _floor.data().items<parameter>())
    {
        _floor.data().put(to_constraint(pair.second));
    }
}

void constraints_builder::to_constraints(
    const corecad::model::registry<model::parameter::parameter> &from,
    corecad::model::registry<model::floor::constraint_t> &to
) const
{
    for (const auto& pair : from)
    {
        to.put(to_constraint(pair.second));
    }
}

model::floor::constraint_t constraints_builder::to_constraint(const model::parameter::parameter &p) const
{
    return std::visit(corecad::util::overloaded
        {
            [&](const parameter::concrete_t<distance>& d) -> model::floor::constraint_t {
                // if distance is specified to wall border points, translate it to axis points
                auto get_wbpl_distance_adjustment = [&](const wall_border_point_locator& wbpl) {
                    const auto& wall = _floor.data().get(wbpl.wid);
                    const auto& axis = _floor.data().get(wall.axis);

                    // get axis point index from border point index
                    auto pid = wbpl.point_on_border_ptr == &wall_border_line::s ? axis.s : axis.e;

                    // adjust distance value
                    const auto wall_orienation = get_wall_orientation_axis(wall);
                    if (!wall_orienation)
                    {
                        throw std::runtime_error("Distance to diagonal walls is not supported");
                    }

                    if (wall_orienation.value().axis == d.direction)
                    {
                        const auto stub_id = wbpl.point_on_border_ptr == &wall_border_line::s ? wall.start_stub : wall.end_stub;
                        if (!stub_id)
                        {
                            throw std::runtime_error("Parallel distance to colinear walls joints is not supported");
                        }
                        else
                        {
                            // Do nothing. Distance adjustment is not necessary.
                            return std::pair<wall_axis_point::index_t, double>(pid, 0.0);
                        }
                    }
                    else
                    {
                        auto adjustment = wbpl.border_ptr == &wall::left
                            ? -(wall.width * 0.5 + wall.axis_offset)
                            : wall.width * 0.5  - wall.axis_offset;
                        
                        if (wall_orienation.value().s == sign::neg)
                        {
                            adjustment = -adjustment;
                        }

                        return std::pair<wall_axis_point::index_t, double>(pid, adjustment);
                    }
                };
                
                auto adjusted_distance = d.value.val();

                auto from_adjustment = std::visit(corecad::util::overloaded{
                    [&](const wall_axis_point_locator& wapl) {
                        return std::pair<wall_axis_point::index_t, double>(std::get<wall_axis_point::index_t>(_pr.resolve(wapl)), 0.0);
                    }, 
                    get_wbpl_distance_adjustment
                }, d.from.val());

                wall_axis_point::index_t pf = from_adjustment.first;
                adjusted_distance += from_adjustment.second;

                auto to_adjustment = std::visit(corecad::util::overloaded{
                    [&](const wall_axis_point_locator& wapl) {
                        return std::pair(std::get<wall_axis_point::index_t>(_pr.resolve(wapl)), 0.0);
                    }, 
                    get_wbpl_distance_adjustment
                }, d.to.val());

                wall_axis_point::index_t pt = to_adjustment.first;
                adjusted_distance -= to_adjustment.second;

                return model::floor::constraint_t::create<offset>(pf, pt, adjusted_distance, d.direction);
            },
            [&](const parameter::concrete_t<colinear>& c) -> model::floor::constraint_t {
                const auto& p1 = _pr.resolve(c.point1);
                const auto& p2 = _pr.resolve(c.point2);
                const auto& p3 = _pr.resolve(c.point3);

                return model::floor::constraint_t::create<aligned>(p1, p2, p3);
            },
            [&](const parameter::concrete_t<pinned>& p) -> model::floor::constraint_t {
                const auto& point = _pr.resolve(p.point);

                return model::floor::constraint_t::create<fixed>(point, p.value, p.coordinate);
            }
        },
        p.instance
    );
}

std::optional<constraints_builder::wall_orientation> constraints_builder::get_wall_orientation_axis(const wall& w) const
{
    const auto& axis = _floor.data().get(w.axis);

    auto distances = _floor.data().items<model::parameter::parameter>()
        | views::take_model_variants<distance>();

    for (const model::parameter::parameter::concrete_t<distance>& d : distances)
    {
        if (d.value != 0.0)
        {
            continue;
        }

        const auto from = _pr.resolve(d.from);
        const auto* from_a = std::get_if<wall_axis_point::index_t>(&from);
        if (!from_a)
        {
            continue;
        }

        const auto to = _pr.resolve(d.to);
        const auto* to_a = std::get_if<wall_axis_point::index_t>(&to);
        if (!to_a)
        {
            continue;
        }

        if (
            ((*from_a) == axis.s || (*to_a) == axis.s) &&
            ((*from_a) == axis.e || (*to_a) == axis.e) &&
            (from != to) // this should never occur. Safety guard
        )
        {
            auto dist = d.value;
            if ((*from_a) == axis.e)
            {
                dist = -dist;
            }

            return wall_orientation
            {
                .axis = (d.direction == coordinate2d::x) ? coordinate2d::y : coordinate2d::x,
                .s = dist > 0.0 ? sign::pos : sign::neg
            }; 
        }
    }

    return std::nullopt;
}

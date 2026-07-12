#include "constraints_builder.h"

#include "overloaded.h"

using namespace domain::plan::calculator;
using namespace domain::plan;
using namespace domain::plan::model::parameter;
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

    // generate wall geometry constraints. For walls, which have no dimension constraints

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
                const auto& pf = _pr.resolve(d.from);
                const auto& pt = _pr.resolve(d.to);

                return model::floor::constraint_t::create<offset>(pf, pt, d.value, d.direction);
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

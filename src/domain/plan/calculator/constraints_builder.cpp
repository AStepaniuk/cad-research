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
        const auto& p = pair.second;

        std::visit(corecad::util::overloaded
            {
                [&](const parameter::concrete_t<distance>& d) {
                    const auto& pf = _pr.resolve(d.from);
                    const auto& pt = _pr.resolve(d.to);

                    if (d.value == 0.0)
                    {
                        _floor.data().put(model::floor::constraint_t::create<offset>(pf, pt, d.value, d.direction));
                    }
                    else
                    {

                    }
                }
            },
            pair.second.instance
        );
    }

    // generate wall geometry constraints. For walls, which have no dimension constraints

}

#include "constraints_builder.h"

using namespace domain::plan::calculator;
using namespace domain::plan;
using namespace domain::plan::model::parameter;

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

        const auto& pf = _pr.resolve(p.from);
        const auto& pt = _pr.resolve(p.to);

        if (p.value == 0.0)
        {
            // vertical or horizontal alignment
            if (p.direction == distance_direction::horizontal)
            {

            }
            else if (p.direction == distance_direction::vertical)
            {}
            else
            {
                std::cerr << "Diagonal alignment is not supported yet" << std::endl;
            }
        }
        else
        {

        }
    }

    // generate wall geometry constraints. For walls, which have no dimension constraints

}

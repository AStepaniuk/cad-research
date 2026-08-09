#include "parameters_redundance_checker.h"

#include "overloaded.h"

using namespace domain::plan::calculator;

parameters_redundance_checker::parameters_redundance_checker(floor_query &fq, resolver::point_resolver& pr)
    : _fq { fq }
    , _pr { pr }
{
}

bool parameters_redundance_checker::is_parameter_redundant(const model::parameter::parameter &p)
{
    return std::visit(corecad::util::overloaded
    {
        [&](const model::parameter::parameter::concrete_t<model::parameter::distance>& d) {
            const auto from = _pr.resolve(d.from);
            const auto to = _pr.resolve(d.to);

            if (_fq.are_points_constrained_on_coordinate(from, to, d.direction))
            {
                return true;
            }

            return false;
        },
        [&](const model::parameter::parameter::concrete_t<model::parameter::colinear>& c) {
            return false;
        },
        [&](const model::parameter::parameter::concrete_t<model::parameter::pinned>& p) {
            return false;
        }
    }, p.instance);
}

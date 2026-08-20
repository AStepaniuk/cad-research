#include "parameters_redundance_checker.h"

#include "overloaded.h"

using namespace domain::plan::calculator;

parameters_redundance_checker::parameters_redundance_checker(floor_query &fq)
    : _fq { fq }
{
}

bool parameters_redundance_checker::is_parameter_redundant(const model::parameter::parameter &p)
{
    return std::visit(corecad::util::overloaded
    {
        [&](const model::parameter::parameter::concrete_t<model::parameter::distance>& d) {
            if (_fq.are_points_constrained_on_coordinate(d.from, d.to, d.direction))
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

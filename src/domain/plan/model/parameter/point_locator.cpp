#include "point_locator.h"

std::ostream &domain::plan::model::parameter::operator<<(std::ostream &os, const point_locator_t &l)
{
    std::visit([&os](auto& impl) {
        os << impl;
    }, l);

    return os;
}

#include "handle_data.h"

#include "type_meta_info.h"

std::ostream &gui::doc::operator<<(std::ostream &os, const handle_data &hd)
{
    os << "hd::  pid:";
    std::visit([&](const auto& id){ os << id; }, hd.handle_id());

    return os;
}
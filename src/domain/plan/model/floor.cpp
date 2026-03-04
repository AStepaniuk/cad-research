#include "floor.h"

using namespace domain::plan::model;

const floor::data_t& floor::data() const
{
    return _data;
}

floor::data_t& floor::data()
{
    return _data;
}

const floor::constraints_t &domain::plan::model::floor::parameters() const
{
    return _parameters;
}

floor::constraints_t &domain::plan::model::floor::parameters()
{
    return _parameters;
}

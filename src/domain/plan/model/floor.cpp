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

floor::history_t& floor::history()
{
    return _history;
}

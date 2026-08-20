#pragma once

#include "parameter.h"
#include "floor_query.h"

namespace domain::plan::calculator
{
    class parameters_redundance_checker
    {
    public:
        parameters_redundance_checker(floor_query& fq);

        bool is_parameter_redundant(const model::parameter::parameter& p);
    
    private:
        floor_query& _fq;
    };
}

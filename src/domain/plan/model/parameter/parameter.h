#pragma once

#include "variant_model_base.h"
#include "distance.h"
#include "colinear.h"
#include "pinned.h"

namespace domain::plan::model::parameter {
    struct parameter;

    using parameter_base = corecad::model::variant_model_base<
        parameter
        , corecad::model::nothing
        , distance<parameter>
        , colinear<parameter>
        , pinned<parameter>
    >;
    
    struct parameter : parameter_base
    {
        template<template <typename> typename TInst>
        using concrete_t = TInst<parameter>;

    protected:
        explicit parameter(typename parameter_base::instance_t i)
            : parameter_base { std::move(i) }
        {
        }

    private:
        friend parameter_base; 
    };
} 

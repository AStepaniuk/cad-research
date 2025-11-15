#pragma once

#include "model_base.h"

namespace corecad { namespace model
{
    class vector2d : public corecad::model::model_base<vector2d>
    {
    public:
        vector2d(double x, double y);

        double x;
        double y;
    };
}}

#pragma once

#include "model_base.h"
#include "vector2d.h" 

namespace domain { namespace plan { namespace model
{
    class wall : public corecad::model::model_base<wall>
    {
    public:
        wall(corecad::model::vector2d::index_t b, corecad::model::vector2d::index_t e, double w);

        // primary model properties
        corecad::model::vector2d::index_t start;
        corecad::model::vector2d::index_t end;

        double width;

        // calculated properties
        corecad::model::vector2d::index_t start_left;
        corecad::model::vector2d::index_t start_right;
        corecad::model::vector2d::index_t end_left;
        corecad::model::vector2d::index_t end_right;

        unsigned int start_joints;
        unsigned int end_joints;
    };  
}}}

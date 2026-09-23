#include "default_floor_generator.h"

#include <iostream>

using namespace domain::plan::generator; 
using namespace domain::plan::model::shape; 
using namespace domain::plan::model::parameter;
using namespace domain::plan; 
using namespace corecad::model;
using namespace corecad::model::constraint;
using namespace mp_units;

void default_floor_generator::generate_floor(model::floor &floor)
{
    floor.data().clear<wall_axis_point>();
    floor.data().clear<wall>();

    auto li = floor.data().put(wall_axis_point { 1000.0 * mm, 1000.0 * mm });
    auto mi = floor.data().put(wall_axis_point { 4000.0 * mm, 1000.0 * mm });
    auto ri = floor.data().put(wall_axis_point { 8000.0 * mm, 1000.0 * mm });

    auto l1 = floor.data().make<wall_axis_line>(li, mi);
    auto w1 = floor.data().make<wall>(l1, 400.0 * mm);

    auto l2 = floor.data().make<wall_axis_line>(mi, ri);
    auto w2 = floor.data().make<wall>(l2, 600.0 * mm);
}

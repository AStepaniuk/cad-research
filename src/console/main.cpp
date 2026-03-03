#include <iostream>

#include "floor.h"
#include "wall.h"

int main()
{
    using namespace corecad::model;
    using namespace domain::plan::model;
    
    wall_axis_point p { 100, 200 };

    domain::plan::model::floor f;
    auto id1 = f.wall_axis_points().put(p);
    auto id2 = f.wall_axis_points().put(p);

    std::cout << "Point ID1: " << id1 << std::endl;
    std::cout << "Point ID2: " << id2 << std::endl;
    std::cout << "p.x: " << f.wall_axis_points().get(id1).x << std::endl;

    const auto& w_id1 = f.walls().make(id1, id2, 10.0f);
    std::cout << "w_id1: " << w_id1 << std::endl;

    return 0;
}
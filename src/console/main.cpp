#include <iostream>

#include "floor.h"
#include "wall.h"

int main()
{
    using namespace corecad::model;
    using namespace domain::plan::model;
    
    vector2d p { 100, 200 };

    floor f;
    auto id1 = f.points().put(p);
    auto id2 = f.points().put(p);

    std::cout << "Point ID1: " << id1 << std::endl;
    std::cout << "Point ID2: " << id2 << std::endl;
    std::cout << "p.x: " << f.points().get(id1).x << std::endl;

    const auto& w_id1 = f.walls().make(id1, id2, 10.0f);
    std::cout << "w_id1: " << w_id1 << std::endl;

    return 0;
}
#include "default_floor_generator.h"

using namespace domain::plan::generator; 
using namespace domain::plan::model; 
using namespace corecad::model;

void default_floor_generator::generate_floor(model::floor &floor)
{
    floor.points().clear();
    floor.walls().clear();

    vector2d tlp { 1000, 1000 }, trp { 8000, 1000 }, blp { 1000, 5000 }, brp { 8000, 5000 };

    auto tli = floor.points().put(tlp);
    auto tri = floor.points().put(trp);
    auto bli = floor.points().put(blp);
    auto bri = floor.points().put(brp);

    floor.walls().make(tli, tri, 400.0f);
    floor.walls().make(tri, bri, 400.0f);
    floor.walls().make(bri, bli, 400.0f);
    floor.walls().make(bli, tli, 400.0f);
}
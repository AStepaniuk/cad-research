#include "default_floor_generator.h"

using namespace domain::plan::generator; 
using namespace domain::plan::model; 
using namespace corecad::model;
using namespace corecad::model::constraint;

void default_floor_generator::generate_floor(model::floor &floor)
{
    floor.points().clear();
    floor.walls().clear();

    vector2d tlp { 1000, 1000 }, trp { 8000, 1000 }, blp { 1000, 5000 }, brp { 8000, 5000 };
    vector2d top { 12000, 1000 }, bop { 10000, 5000 };

    auto tli = floor.points().put(tlp);
    auto tri = floor.points().put(trp);
    auto bli = floor.points().put(blp);
    auto bri = floor.points().put(brp);
    auto toi = floor.points().put(top);
    auto boi = floor.points().put(bop);

    floor.walls().make(tli, tri, 400.0);
    floor.walls().make(tri, bri, 400.0);
    floor.walls().make(bri, bli, 400.0);
    floor.walls().make(bli, tli, 400.0);

    floor.walls().make(tri, toi, 400.0);
    floor.walls().make(bri, boi, 400.0);

    floor.parameters().put(fixed { tli, 1000, fixed_coordinate::x });
    floor.parameters().put(fixed { tli, 1000, fixed_coordinate::y });
    floor.parameters().put(offset { tli, tri, 10000.0, offset_direction::horizontal });
    floor.parameters().put(offset { tli, tri, 0.0, offset_direction::vertical });
    //floor.parameters().put(offset { tri, toi, 2000.0, offset_direction::horizontal });
    floor.parameters().put(offset { tri, toi, 1000.0, offset_direction::vertical });
    //floor.parameters().put(offset { tli, bli, 1000.0, offset_direction::horizontal });
    //floor.parameters().put(offset { tli, bli, 5000.0, offset_direction::vertical });
    //floor.parameters().put(offset { tri, bri, 0.0, offset_direction::horizontal });
    floor.parameters().put(offset { bli, bri, 0.0, offset_direction::vertical });
    floor.parameters().put(offset { bri, boi, 2000.0, offset_direction::horizontal });
    floor.parameters().put(offset { bri, boi, 0.0, offset_direction::vertical });
}
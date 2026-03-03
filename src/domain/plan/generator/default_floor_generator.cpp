#include "default_floor_generator.h"

using namespace domain::plan::generator; 
using namespace domain::plan::model; 
using namespace corecad::model;
using namespace corecad::model::constraint;

void default_floor_generator::generate_floor(model::floor &floor)
{
    floor.wall_axis_points().clear();
    floor.walls().clear();

    wall_axis_point tlp { 1000, 1000 }, trp { 8000, 1000 }, blp { 1000, 5000 }, brp { 8000, 5000 };
    wall_axis_point top { 12000, 1000 }, bop { 10000, 5000 };

    auto tli = floor.wall_axis_points().put(tlp);
    auto tri = floor.wall_axis_points().put(trp);
    auto bli = floor.wall_axis_points().put(blp);
    auto bri = floor.wall_axis_points().put(brp);
    auto toi = floor.wall_axis_points().put(top);
    auto boi = floor.wall_axis_points().put(bop);

    floor.walls().make(tli, tri, 400.0);
    floor.walls().make(tri, bri, 400.0);
    floor.walls().make(bri, bli, 400.0);
    floor.walls().make(bli, tli, 400.0);

    floor.walls().make(tri, toi, 400.0);
    floor.walls().make(bri, boi, 400.0);

    //floor.parameters().put(fixed { tli, 1000, fixed_coordinate::x });
    //floor.parameters().put(fixed { tli, 1000, fixed_coordinate::y });
    floor.parameters().put(offset<wall_axis_point> { tli, tri, 10000.0, offset_direction::horizontal });
    floor.parameters().put(offset<wall_axis_point> { tli, tri, 0.0, offset_direction::vertical });
    //floor.parameters().put(offset { tri, toi, 2000.0, offset_direction::horizontal });
    floor.parameters().put(offset<wall_axis_point> { tri, toi, 1000.0, offset_direction::vertical });
    //floor.parameters().put(offset { tli, bli, 1000.0, offset_direction::horizontal });
    //floor.parameters().put(offset { tli, bli, 5000.0, offset_direction::vertical });
    //floor.parameters().put(offset { tri, bri, 0.0, offset_direction::horizontal });
    floor.parameters().put(offset<wall_axis_point> { bli, bri, 0.0, offset_direction::vertical });
    floor.parameters().put(offset<wall_axis_point> { bri, boi, 2000.0, offset_direction::horizontal });
    floor.parameters().put(offset<wall_axis_point> { bri, boi, 0.0, offset_direction::vertical });
}
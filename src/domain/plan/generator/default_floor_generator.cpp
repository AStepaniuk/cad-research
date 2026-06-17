#include "default_floor_generator.h"

#include <iostream>

using namespace domain::plan::generator; 
using namespace domain::plan::model::shape; 
using namespace domain::plan::model::parameter;
using namespace domain::plan; 
using namespace corecad::model;
using namespace corecad::model::constraint;

void default_floor_generator::generate_floor(model::floor &floor)
{
    floor.data().clear<wall_axis_point>();
    floor.data().clear<wall>();

    wall_axis_point tlp { 1000, 1000 }, trp { 8000, 1000 }, blp { 1000, 5000 }, brp { 8000, 5000 };
    wall_axis_point top { 12000, 1000 }, bop { 10000, 5000 };

    auto tli = floor.data().put(tlp);
    auto tri = floor.data().put(trp);
    auto bli = floor.data().put(blp);
    auto bri = floor.data().put(brp);
    //auto toi = floor.data().put(top);
    //auto boi = floor.data().put(bop);

    auto l1 = floor.data().make<wall_axis_line>(tli, tri);
    auto w1 = floor.data().make<wall>(l1, 600.0);
    floor.data().get(w1).axis_offset = -100.0;
    //floor.data().make<wall>(tri, bri, 400.0);
    auto l2 = floor.data().make<wall_axis_line>(bri, bli);
    auto w2 = floor.data().make<wall>(l2, 400.0);
    //floor.data().make<wall>(bli, tli, 400.0);

    //floor.data().make<wall>(tri, toi, 400.0);
    //floor.data().make<wall>(bri, boi, 400.0);

    floor.data().put(model::floor::constraint_t::create<model::floor::fixed_wall_axis_point_t>(tli, 1000.0, fixed_coordinate::x));
    floor.data().put(model::floor::constraint_t::create<model::floor::fixed_wall_axis_point_t>(tli, 1000.0, fixed_coordinate::y));
    auto c1 = floor.data().put(model::floor::constraint_t::create<model::floor::offset_wall_axis_point_t>(tli, tri, 10000.0, offset_direction::horizontal));
    //floor.data().put(floor::constraint_t::create<floor::offset_wall_axis_point_t>(tli, tri, 0.0, offset_direction::vertical));
    //floor.parameters().put(offset { tri, toi, 2000.0, offset_direction::horizontal });
    //floor.data().put(floor::constraint_t::create<floor::offset_wall_axis_point_t>(tri, toi, 1000.0, offset_direction::vertical));
    //floor.parameters().put(offset { tli, bli, 1000.0, offset_direction::horizontal });
    //floor.parameters().put(offset { tli, bli, 5000.0, offset_direction::vertical });
    //floor.parameters().put(offset { tri, bri, 0.0, offset_direction::horizontal });
    //floor.data().put(floor::constraint_t::create<floor::offset_wall_axis_point_t>(bli, bri, 0.0, offset_direction::vertical));
    //floor.data().put(floor::constraint_t::create<floor::offset_wall_axis_point_t>(bri, boi, 2000.0, offset_direction::horizontal));
    //floor.data().put(floor::constraint_t::create<floor::offset_wall_axis_point_t>(bri, boi, 0.0, offset_direction::vertical));

    auto p1 = floor.data().make<parameter>(
        wall_border_point_locator { w1, &wall::right, &wall_border_line::s },
        wall_border_point_locator { w2, &wall::right, &wall_border_line::e },
        5000.0,
        distance_direction::vertical
    );
}

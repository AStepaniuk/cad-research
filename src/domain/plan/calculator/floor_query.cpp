#include "floor_query.h"

#include <unordered_set>
#include <ranges>

#include "views_take_model_variants.h"
#include "members_iterator.h"

using namespace domain::plan::calculator;
using namespace domain::plan::model::parameter;
using namespace domain::plan::model::shape;
using namespace domain::plan;
using namespace corecad::model;

floor_query::floor_query(model::floor &floor)
    : _floor { floor }
{
}

std::vector<point_locator_t> floor_query::find_locators(wall_axis_point::index_t pid)
{
    std::vector<point_locator_t> result;

    for (const auto& wp : _floor.data().items<wall>())
    {
        const auto& al = _floor.data().get(wp.second.axis);

        if (al.s == pid)
        {
            result.push_back(wall_axis_point_locator { wp.first, &wall_axis_line::s });
        }

        if (al.e == pid)
        {
            result.push_back(wall_axis_point_locator { wp.first, &wall_axis_line::e });
        }
    }

    return result;
}

using point_id_t = model::floor::constraint_t::point_id_t;

template <typename T>
using is_point_id_property = corecad::model::is_property_of_type<T, point_id_t>;

bool floor_query::are_points_constrained_on_coordinate(const point_id_t &p1, const point_id_t &p2, coordinate2d coord)
{
    auto offsets_pairs = _floor.data().items<model::floor::constraint_t>()
        | views::take_model_variants_with_ids<constraint::offset>()
        | std::views::filter([coord](const auto& pair) { return pair.second.direction == coord; })
        | std::ranges::to<std::vector>();

    std::unordered_set<model::floor::constraint_t::index_t> visited_constraints;
    
    auto check_if_p2_constrained = [&](this auto& self, const auto& pid) -> bool {
        std::cout << "checking points: " << pid << " to " << p2 << std::endl;

        for (const auto& pair : offsets_pairs)
        {
            std::cout << "checking contraint: " << pair.second << std::endl;
            if (visited_constraints.contains(pair.first))
            {
                continue;
            }
            
            if (pair.second.from == pid)
            {
                if (pair.second.to == p2)
                {
                    return true;
                }

                visited_constraints.insert(pair.first);
                if (self(pair.second.to))
                {
                    return true;
                }
            }

            if (pair.second.to == pid)
            {
                if (std::ranges::contains(visited_constraints, pair.first))
                {
                    continue;
                }

                if (pair.second.from == p2)
                {
                    return true;
                }

                visited_constraints.insert(pair.first);
                if (self(pair.second.to))
                {
                    return true;
                }
            }
        }
        
        return false;
    };

    std::cout << "checking points constraint: " << p1 << " to " << p2 << std::endl;
    auto res = check_if_p2_constrained(p1);
    std::cout << "checking points result: " << res << std::endl;
    std::cout << "coffsets_pairs.size()=" << offsets_pairs.size() << std::endl;
    
    return res;
}

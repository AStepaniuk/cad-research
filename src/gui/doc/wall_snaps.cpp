#include "wall_snaps.h"

#include <algorithm>

using namespace gui::doc;
using namespace domain::plan::model;
using namespace corecad::model;
using namespace corecad::model::constraint;

gui::doc::wall_snaps::wall_snaps(const corecad::model::registry<domain::plan::model::wall_axis_point> &wall_axis_points)
    : _wall_axis_points { wall_axis_points }
{
}

void wall_snaps::clear()
{
    _constraints.clear();
    _ranks.clear();

    _affected_points.clear();
    _affected_points_data.clear();
}

void gui::doc::wall_snaps::ease()
{
    if (_ranks.empty())
    {
        return;
    }

    const auto it = std::ranges::max_element(_ranks, {}, &rank_data::second);
    const auto cid = it->first;

    _constraints.erase(cid);
    _ranks.erase(it);

    const auto apit = _affected_points.find(cid);
    for (const auto pid : apit->second)
    {
        const auto refit = _affected_points_data.find(pid);
        if (refit->second.refcount > 1)
        {
            (refit->second.refcount)--;
        }
        else
        {
            _constraints.erase(refit->second.h_fix);
            _constraints.erase(refit->second.v_fix);
            _affected_points_data.erase(refit);
        }
    }
    _affected_points.erase(apit);
}

const registry<floor::constraint_t>& wall_snaps::constraints() const
{
    return _constraints;
}

void wall_snaps::clone_active_handle_constraints(registry<floor::constraint_t>& dest) const
{
    // clone only ranked constraints
    for (auto rp : _ranks)
    {
        dest.put(_constraints.get(rp.first));
    }
}

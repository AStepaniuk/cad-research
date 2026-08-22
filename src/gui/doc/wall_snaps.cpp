#include "wall_snaps.h"

#include <algorithm>

using namespace gui::doc;
using namespace domain::plan::model;
using namespace corecad::model;
using namespace corecad::model::constraint;

wall_snaps::wall_snaps(const floor::data_t& data)
    : _data { data }
{
}

void wall_snaps::clear()
{
    _parameters.clear();
    _ranks.clear();
    _anchors.clear();

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

    _parameters.erase(cid);
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
            _anchors.erase(refit->second.h_fix);
            _anchors.erase(refit->second.v_fix);
            _affected_points_data.erase(refit);
        }
    }
    _affected_points.erase(apit);
}

const registry<wall_snaps::parameter_t>& wall_snaps::parameters() const
{
    return _parameters;
}

const corecad::model::registry<wall_snaps::constraint_t> &gui::doc::wall_snaps::anchors() const
{
    return _anchors;
}

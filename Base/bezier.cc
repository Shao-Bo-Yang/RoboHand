#include "bezier.hh"

#include <utility>
#include <vector>

namespace base
{

void bezier(const std::vector<point> &control_points, float ratio, point &out)
{
    std::vector<point> prev_points = control_points;
    while (prev_points.size() > 1)
    {
        std::vector<point> current_control_points;
        current_control_points.reserve(prev_points.size() - 1);
        for (auto i = prev_points.begin() + 1; i != prev_points.end(); ++i)
        {
            auto j = i - 1;
            current_control_points.push_back(point{(i->x - j->x) * ratio + j->x, (i->y - j->y) * ratio + j->y});
        }
        prev_points = std::move(current_control_points);
    }
    if (prev_points.size() == 1)
    {
        out = std::move(prev_points.front());
    }
}

} // namespace base

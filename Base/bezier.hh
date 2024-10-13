#pragma once

#include <vector>
namespace base
{

struct point
{
    float x = 0;
    float y = 0;
};

extern void bezier(const std::vector<point> &control_points, float ratio, point &out);

} // namespace base

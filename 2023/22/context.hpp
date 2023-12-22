#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    struct brick
    {
        int64_t start_x, start_y, start_z, end_x, end_y, end_z;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<brick> bricks;
    };
}

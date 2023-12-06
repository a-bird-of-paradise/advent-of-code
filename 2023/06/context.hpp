#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<uint64_t> time, distance;
    };
}

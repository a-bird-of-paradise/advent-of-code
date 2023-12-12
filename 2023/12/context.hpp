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
        std::vector<std::vector<char>> field;
        std::vector<std::vector<int64_t>> contiguous;
    };
}

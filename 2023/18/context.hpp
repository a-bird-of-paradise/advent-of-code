#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    struct point 
    { 
        int64_t x, y;
        auto operator<=>(const point&) const = default;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<char> dir;
        std::vector<int64_t> length;
        std::vector<std::string> colour;
    };
}

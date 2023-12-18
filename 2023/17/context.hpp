#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    enum class direction { up, down, left, right };

    struct point { 
        int64_t r, c; 
        auto operator<=>(const point&) const = default;
    };

    struct state {
        point p;
        direction dir;
        auto operator<=>(const state&) const = default;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::vector<int64_t>> field;
    };
}

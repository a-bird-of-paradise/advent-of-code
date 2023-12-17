#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    enum class direction { up, down, left, right };

    struct point { int32_t r, c; 
    auto operator<=>(const point&) const = default;};

    struct beam { direction dir; point p; };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::string> field;
    };
}

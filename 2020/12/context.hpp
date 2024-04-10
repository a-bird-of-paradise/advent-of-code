#pragma once
#include "location.hh"

template<typename V>
V mod(const V& a, const V& b)
{
    return (a % b + b) % b;
}

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class context
    {
    public:
        context() : x(0), y(0), angle(0), wx(10), wy(1), sx(0), sy(0) { loc.initialize(); }
        location loc;
        int64_t x, y, angle;
        int64_t wx, wy, sx, sy;
    };
}

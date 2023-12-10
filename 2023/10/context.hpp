#pragma once
#include "location.hh"
#include <map>

namespace aoc
{

    class point 
    { 
    public:
        point(long x_, long y_) : x(x_), y(y_) {}
        point() : x(0), y(0) {}
        auto operator<=>(const point&) const = default;
        long x, y; 
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::map<point,char> pipes;
    };
}

#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    class point
    {
    public:
        point() : x(0),y(0),z(0) {}
        point(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
        friend auto operator<=>(const point&, const point&) = default;

        int x, y, z;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::vector<point>> scanners;
    };

    class fingerprint
    {
    public:
    fingerprint(const point& x, const point& y) ;
    fingerprint() : sumsq(0), manhattan(0) {} ;

    friend auto operator<=>(const fingerprint&, const fingerprint&) = default;

    long long sumsq, manhattan;
    };
}

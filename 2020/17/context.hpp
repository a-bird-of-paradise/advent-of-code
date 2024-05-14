#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class point { 
    public:
        point() : x(0), y(0), z(0) {}
        point(int32_t x_, int32_t y_, int32_t z_) { x=x_; y=y_; z=z_;}
        auto operator<=>(const point&) const = default;
        int32_t x; 
        int32_t y;
        int32_t z;
    };

    class point4 { 
    public:
        point4() : w(0), x(0), y(0), z(0) {}
        point4(int32_t w_, int32_t x_, int32_t y_, int32_t z_) { w=w_;x=x_; y=y_; z=z_;}
        auto operator<=>(const point4&) const = default;
        int32_t w;
        int32_t x; 
        int32_t y;
        int32_t z;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::string> lines;
    };
}

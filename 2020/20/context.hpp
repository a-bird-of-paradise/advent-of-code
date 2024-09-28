#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{

    typedef std::vector<std::string> tile;

    struct tile_state { 
        int t; 
        int r; 
        tile_state() : t(0), r(0) {} 
        tile_state(int t_, int r_) : t(t_), r(r_) {} 
        auto operator<=>(const tile_state&) const = default;
    } ;

    typedef std::vector<std::vector<aoc::tile_state>> field_t;

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::map<int, aoc::tile> tiles;

    };
}

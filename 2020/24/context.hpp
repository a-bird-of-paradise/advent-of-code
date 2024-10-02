#pragma once
#include "location.hh"
#include <set>

namespace aoc
{
    struct move
    {
        move() : e(0),nw(0) {}
        move(int e_, int nw_) : e(e_), nw(nw_) {}
        auto operator<=>(const move&) const = default;
        move operator + (const move & right) { return move(e + right.e, nw + right.nw);}
        move operator - (const move & right) { return move(e - right.e, nw - right.nw);}
        move operator - () { return move(-e, -nw);  }
        friend std::ostream& operator<<(std::ostream& os, const aoc::move& dt);
        int e;
        int nw;
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::set<aoc::move> flipped;
    };
}


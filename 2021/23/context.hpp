#pragma once
#include "location.hh"
#include <vector>
#include <stack>

namespace aoc
{

    enum class state { amber, bronze, copper, desert, vacant, unoccupiable };

    class board
    {
    public:

        board();

        auto operator<=>(const board&) const = default;

        unsigned long cost;
        state hallway[11];
        state pockets[4][4];
    };
    class board_priority
    {
        public:
        bool operator()(const aoc::board& l, const aoc::board& r) const { return l.cost > r.cost; }
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<char> letters;
    };
}

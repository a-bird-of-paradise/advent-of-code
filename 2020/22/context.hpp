#pragma once
#include "location.hh"
#include <deque>

namespace aoc
{

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::deque<int> p1, p2;
    };
}

#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    class context
    {
    public:
        context() : answer_1(0), answer_2(0) { loc.initialize(); }
        location loc;
        long answer_1, answer_2;
    };
}

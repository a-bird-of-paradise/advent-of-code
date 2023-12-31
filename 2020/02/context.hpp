#pragma once
#include "location.hh"

namespace aoc
{

    class context
    {
    public:
        context() : answer_1(0), answer_2(0) { loc.initialize(); }
        location loc;
        int64_t answer_1, answer_2;
    };
}

#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{
    class context
    {
    public:
        context() : answer_1(0), answer_2(0) { loc.initialize(); }
        location loc;
        long answer_1, answer_2, counter;
        std::vector<long> winners;
        std::map<long,long> win_pile;
    };
}

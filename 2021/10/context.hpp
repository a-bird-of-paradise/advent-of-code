#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    extern int error_state;
    extern unsigned long long answer_2;

    class context
    {
    public:
        context() : answer_1(0) { loc.initialize();   }
        location loc;
        unsigned long long answer_1;
        std::vector<unsigned long long> line_totals;
    };
}
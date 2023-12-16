#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    typedef std::vector<std::string> note;

    class context
    {
    public:
        context() : answer_1(0), answer_2(0), counter(0) { loc.initialize(); }
        location loc;
        std::vector<note> notes;
        int64_t answer_1, answer_2, counter;
    };
}

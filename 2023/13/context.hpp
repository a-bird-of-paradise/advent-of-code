#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    typedef std::vector<std::vector<char>> note;

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<note> notes;
    };
}

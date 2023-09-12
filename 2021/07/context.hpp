#pragma once
#include "location.hh"

namespace aoc
{
    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<long> list;
    };
}
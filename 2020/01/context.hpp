#pragma once
#include "location.hh"
#include <set>

namespace aoc
{

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::set<int64_t> numbers;
    };
}

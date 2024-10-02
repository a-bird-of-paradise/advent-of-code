#pragma once
#include "location.hh"

namespace aoc
{
    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        uint64_t key_1, key_2;
    };
}


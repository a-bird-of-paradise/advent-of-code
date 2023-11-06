#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    class context
    {
    public:
        context() : current_iter(0) { loc.initialize(); }
        location loc;
        std::vector<uint8_t> algo;
        std::vector<std::vector<uint8_t>> field;
        unsigned long long current_iter;
    };

}

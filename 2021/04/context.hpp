#pragma once
#include "location.hh"

namespace aoc
{

    typedef std::vector<std::vector<long>> board_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        std::vector<long> random_numbers;
        std::vector<board_t> boards;
        location loc;

    };
}
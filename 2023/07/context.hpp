#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    enum class hand_type {
        high_card,
        one_pair,
        two_pair,
        three_of_a_kind,
        full_house,
        four_of_a_kind,
        five_of_a_kind
    };

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::pair<std::string,uint64_t>> lines;
    };
}

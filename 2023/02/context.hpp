#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    enum class colour { RED, BLUE, GREEN } ;

    typedef std::pair<colour,int> turn_t;
    typedef std::vector<std::vector<turn_t>> turnlist_t;

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::pair<long,turnlist_t>> file;
    };
}

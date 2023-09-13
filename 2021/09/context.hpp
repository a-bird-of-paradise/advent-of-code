#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    typedef int int_t;
    typedef std::vector<int_t> line_t;
    typedef std::pair<std::size_t,std::size_t> point_t;

    const int_t int_t_max = INT16_MAX;
    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<line_t> lines;
    };
}
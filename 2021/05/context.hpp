#pragma once
#include "location.hh"

namespace aoc
{

    typedef std::pair<long,long> point_t;
    typedef std::pair<point_t,point_t> line_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<line_t> lines;
    };
}
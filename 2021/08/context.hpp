#pragma once
#include "location.hh"

namespace aoc
{

    typedef std::vector<unsigned> list_t;
    typedef std::pair<list_t,list_t> line_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<line_t> lines;
    };
}
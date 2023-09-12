#pragma once
#include "location.hh"

namespace aoc
{

    typedef std::vector<std::string> list_t;
    typedef std::pair<list_t,list_t> line_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<line_t> lines;
    };
}
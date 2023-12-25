#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::map<std::string,std::vector<std::string>> graph;
    };
}

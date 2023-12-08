#pragma once
#include "location.hh"
#include <map>

namespace aoc
{

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::string lr;
        std::map<std::string, std::pair<std::string,std::string>> nodes;
    };
}

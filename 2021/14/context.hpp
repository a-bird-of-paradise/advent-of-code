#pragma once
#include "location.hh"
#include <vector>
#include <tuple>

namespace aoc
{

    typedef std::tuple<char,char,char> rule_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::string Template;
        std::vector<rule_t> rules;
    };
}
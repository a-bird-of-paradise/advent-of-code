#pragma once
#include "location.hh"
#include <stack>
#include <map>
#include <vector>

namespace aoc
{
    class context
    {
    public:
        context() : ticker(0) { loc.initialize(); }
        location loc;
        std::map<char,long long> registers;
        std::stack<long long> input;

        std::vector<long long> A, B, d;
        unsigned int ticker;
    };
}

#pragma once
#include "location.hh"
#include "Tree.hpp"
#include <vector>

namespace aoc
{
    class context
    {
    public:
        context() 
            { 
                loc.initialize(); 
            }
        location loc;
        std::vector<std::unique_ptr<BranchNode>> SnailNumbers;
    };
}
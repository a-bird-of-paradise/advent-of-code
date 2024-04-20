#pragma once
#include "location.hh"
#include <vector>
#include <map>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        uint64_t ones_mask, zero_mask, floats;
        std::map<uint64_t,uint64_t> memory, memory_2;
    };
}

#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.
    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::vector<std::string>> groups;
    };
}

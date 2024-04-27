#pragma once
#include "location.hh"
namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
    };
}

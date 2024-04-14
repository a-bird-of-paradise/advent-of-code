#pragma once
#include "location.hh"
#include <vector>

template<typename V>
V mod(const V& a, const V& b)
{
    return (a % b + b) % b;
}

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<int64_t> line;
        int64_t minutes;
    };
}

#pragma once
#include "location.hh"
#include <boost/icl/interval_set.hpp>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;
        std::vector<std::vector<int64_t>> nearby_tickets;
        std::vector<int64_t> ticket;
        std::vector<std::string> names;
        std::vector<boost::icl::interval_set<int64_t>> ranges;
    };
}

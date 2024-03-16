#pragma once
#include "location.hh"
#include <map>
#include <vector>

namespace aoc
{
    /// Shared state object between Flex scanner, Bison parser and `main()`.

    typedef std::pair<std::string,std::string> bag_colour_t;
    typedef std::pair<bag_colour_t,int32_t> bag_count_t;

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::map<
            bag_colour_t,
            std::vector<bag_count_t>
        >   bags;

        std::map<
            bag_colour_t,
            std::vector<bag_colour_t>
        >   parents;
    };
}

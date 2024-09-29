#pragma once
#include "location.hh"
#include <vector>
#include <set>

namespace aoc
{

    class context
    {
    public:
        context() { loc.initialize(); }
        location loc;

        std::vector<
            std::pair<
                std::set<std::string>,
                std::set<std::string>
            >
        >   lines;
    };
}

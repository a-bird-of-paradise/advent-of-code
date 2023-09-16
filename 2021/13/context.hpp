#pragma once
#include "location.hh"
#include <vector>

namespace aoc
{

    typedef std::pair<std::size_t,std::size_t> point_t;
    typedef std::pair<char,std::size_t> fold_t;

    class context
    {
    public:
        context() { loc.initialize();   }
        location loc;
        std::vector<point_t> points;
        std::vector<fold_t> folds;
    };
}